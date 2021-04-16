#include "nimble/render-passes/PhongPass.h"
#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/scene-graph/DrawableNode.h"
#include "nimble/scene-graph/SceneGraph.h"

using namespace Nimble;

PhongPass::PhongPass() {
	_shader = ResourceManager::Get().GetShader("phong");
	ASSERT(_shader, "Could not find phong shader through resource manager");
}

void PhongPass::Draw(SceneState &state, const SceneGraph &sceneGraph) {
	// Find all drawable nodes
	std::list<DrawableNode *> drawables = sceneGraph.GetNodesByDerivedType<DrawableNode>(SceneNodeType::DRAWABLE);

	// See if we have a directional light in the scene
	DirectionalLightNode *directionalLightNode =
		sceneGraph.GetOneNodeByDerivedType<DirectionalLightNode>(SceneNodeType::DIRECTIONAL_LIGHT);

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

		auto texture = diffuse_texture_unit->texture;
		auto sampler_settings = diffuse_texture_unit->sampler;
		auto receives_lighting = drawable->GetMaterial()->GetReceivesLighting().value();
		auto transform = drawable->GetTransformation();

		_shader->Use();
		_shader->SetUniform("Model", transform.GetWorldMatrix());
		_shader->SetUniform("View", state.GetCamera()->GetView());
		_shader->SetUniform("Projection", *(state.GetProjectionMatrix()));

		if(directionalLightNode && receives_lighting) {
			auto light = directionalLightNode->GetDirectionalLight();
			_shader->SetUniform("lightDirection", light.direction);
			_shader->SetUniform("lightColor", light.color);
			_shader->SetUniform("viewPos", state.GetCamera()->GetPosition());
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_texture_unit->texture->GetTextureHandle());
		diffuse_texture_unit->sampler.Bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(drawable->GetIB().GetNumFaces() * 3), GL_UNSIGNED_INT, nullptr);
		drawable->GetVAO()->Unbind();
	}
}
