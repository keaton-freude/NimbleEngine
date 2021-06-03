#include "nimble/render-passes/PhongPass.h"
#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/scene-graph/DrawableNode.h"
#include "nimble/scene-graph/SceneGraph.h"

using namespace Nimble;

PhongPass::PhongPass() {
	_shader = ResourceManager::Get().GetShader("phong");
	ASSERT(_shader, "Could not find phong shader through resource manager");

	_model_uniform_location = _shader->GetUniformLocation("Model");
	_view_uniform_location = _shader->GetUniformLocation("View");
	_projection_uniform_location = _shader->GetUniformLocation("Projection");
	_uv_multiplier_uniform_location = _shader->GetUniformLocation("UvMultiplier");
	_view_pos_uniform_location = _shader->GetUniformLocation("viewPos");
	_lighting_enabled_uniform_location = _shader->GetUniformLocation("lightingEnabled");
	_diffuse_texture_uniform_location = _shader->GetUniformLocation("diffuse_texture");
	_light_pos_uniform_location = _shader->GetUniformLocation("lightPos");
	_light_space_matrix_uniform_location = _shader->GetUniformLocation("lightSpaceMatrix");
	_shadow_map_uniform_location = _shader->GetUniformLocation("shadow_map");
}

void PhongPass::Draw(SceneState &state, const SceneGraph &sceneGraph) {
	// Find all drawable nodes
	std::list<DrawableNode *> drawables = sceneGraph.GetNodesByDerivedType<DrawableNode>(SceneNodeType::DRAWABLE);

	// See if we have a directional light in the scene
	DirectionalLightNode *directionalLightNode =
		sceneGraph.GetOneNodeByDerivedType<DirectionalLightNode>(SceneNodeType::DIRECTIONAL_LIGHT);

	// directionalLightNode->Apply(state);

	// Walk every drawable node and draw them

	// TODO: Only draw nodes with a Phong material attached

	for(const auto &drawable : drawables) {
		/*
		 * We require the following to draw via this pass:
		 * 	- VAO
		 * 	- VB
		 * 	- IB
		 * 	- Phong Material type
		 * 	- Transform
		 */
		drawable->GetVAO()->Bind();
		drawable->GetVB().Bind();
		drawable->GetIB().Bind();

		/*
		 * From the Phong material we require:
		 * 	- Diffuse Texture
		 * 	- Sampler Settings
		 * 	- Whether the object receives lighting
		 */

		auto diffuse_texture_unit = drawable->GetMaterial()->GetDiffuseTexture();
		ASSERT(diffuse_texture_unit.has_value(), "Phong Pass requires a diffuse texture to be set!");

		auto receives_lighting = drawable->GetMaterial()->GetReceivesLighting().value();
		auto transform = drawable->GetTransformation();
		auto uv_multiplier = drawable->GetMaterial()->GetUvMultiplier();

		_shader->Use();
		_shader->SetUniform(_model_uniform_location, transform.GetWorldMatrix());
		_shader->SetUniform(_view_uniform_location, state.GetCamera()->GetView());
		_shader->SetUniform(_projection_uniform_location, *(state.GetProjectionMatrix()));
		_shader->SetUniform(_uv_multiplier_uniform_location, uv_multiplier.value());

		if(directionalLightNode && receives_lighting) {
			auto light = directionalLightNode->GetDirectionalLight();
			_shader->SetUniform(_view_pos_uniform_location, state.GetCamera()->GetPosition());
			_shader->SetUniform(_lighting_enabled_uniform_location, true);
		} else {
			_shader->SetUniform(_lighting_enabled_uniform_location, false);
		}

		_shader->SetUniform(_diffuse_texture_uniform_location, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_texture_unit->texture->GetTextureHandle());
		diffuse_texture_unit->sampler.Bind();

		if(_shadow_map.depth_texture) {

			auto lightPosition = directionalLightNode->GetDirectionalLight().direction;
			lightPosition *= -1.0f;
			lightPosition *= 10.0f;

			_shader->SetUniform(_light_pos_uniform_location, lightPosition);

			_shader->SetUniform(_light_space_matrix_uniform_location, _shadow_map.light_space_matrix);
			_shader->SetUniform(_shadow_map_uniform_location, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _shadow_map.depth_texture->GetTextureHandle());
		}

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(drawable->GetIB().GetNumFaces() * 3), GL_UNSIGNED_INT, nullptr);
		drawable->GetVAO()->Unbind();
	}
}
