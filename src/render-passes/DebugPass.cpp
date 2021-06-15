#include "nimble/render-passes/DebugPass.h"
#include <nimble/scene-graph/DirectionalLightNode.h>
#include <nimble/utility/ImGuiUtility.h>

using namespace Nimble;

DebugPass::DebugPass()
: _shadow_frustum_node(DrawableNode(ResourceManager::Get().GetMesh<PositionColor>("cube.fbx", "frustum_cube").get(), "debug_frustum")),
  _color_shader(ResourceManager::Get().GetShader("color")) {
	ASSERT(_color_shader, "Could not find color shader");
}

void DebugPass::Draw(SceneState &state, const SceneGraph &sceneGraph) {
	// Variety of optional features which can help in debugging issues
	GUI_CHECKBOX(renderNormals, false);
	GUI_CHECKBOX(showLights, false);
	GUI_CHECKBOX(showShadowFrustrum, false);

	if(showLights) {
		DrawLights(state, sceneGraph);
	}
	if(showShadowFrustrum) {
		DrawShadowFrustrum(state, sceneGraph);
	}
}
void DebugPass::DrawLights(SceneState &state, const SceneGraph &sceneGraph) {
	// Walk every light, depending on the type draw an indicator in world space
	// Directional lights are represented by a line drawn through the scene
	// Spot lights are represented by a location in world space + direction, etc.

	auto directionalLights = sceneGraph.GetNodesByDerivedType<DirectionalLightNode>(SceneNodeType::DIRECTIONAL_LIGHT);

	for(const auto &light : directionalLights) {
		// Draw an extremely long line along the direction vector of the light
		glm::vec3 pos1 = light->GetDirectionalLight().direction * -20.0f;
		glm::vec3 pos2 = light->GetDirectionalLight().direction * 20.0f;
		std::shared_ptr<VertexArrayObject> vao = std::make_shared<VertexArrayObject>(PositionColor::SetVertexAttribPointers);

		Mesh<PositionColor> mesh(std::vector<PositionColor>{ { pos1, { 1.0f, 1.0f, 1.0f, 1.0f } },
															 { pos2, { 1.0f, 1.0f, 1.0f, 1.0f } } },
								 { 0, 1 },
								 1,
								 vao);

		DrawableNode drawable(&mesh, "debug_line");

		drawable.GetVAO()->Bind();
		drawable.GetVB().Bind();
		drawable.GetIB().Bind();

		// Model identity
		glm::mat4 model(1.0f);
		auto shader = ResourceManager::Get().GetShader("color");
		ASSERT(shader, "Could not find color shader");

		shader->Use();
		shader->SetUniform("Model", model);
		shader->SetUniform("View", state.GetCamera()->GetView());
		shader->SetUniform("Projection", *(state.GetProjectionMatrix()));

		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
		drawable.GetVAO()->Unbind();
	}
}
void DebugPass::DrawShadowFrustrum(SceneState &state, const SceneGraph &sceneGraph) {
	// For each light, draw its shadow frustrum

	// For a directional light, it uses an orthographic projection. The light itself
	// contains this projection (updated every frame in case of changes to the lights
	// direction).
	// The projection also includes a transform of its position in order to better fit
	// the scene as the shadow-maps dimensions have cutoff points
	auto directionalLights = sceneGraph.GetNodesByDerivedType<DirectionalLightNode>(SceneNodeType::DIRECTIONAL_LIGHT);

	for(const auto &light : directionalLights) {
		_shadow_frustum_node.GetVAO()->Bind();
		_shadow_frustum_node.GetVB().Bind();
		_shadow_frustum_node.GetIB().Bind();

		const auto pos = light->GetDirectionalLight().position;
		const auto dir = light->GetDirectionalLight().direction;
		const auto proj = light->GetDirectionalLight().projection;
		const auto center = (pos + dir);

		// Translate & Rotate such that we are at the lights position and pointing along
		// the lights direction
		auto scale = glm::scale(glm::vec3(proj.width / 2.f, proj.height / 2.f, proj.depth / 2.f));

		glm::vec3 const f(glm::normalize(center - pos));
		glm::vec3 const s(glm::normalize(glm::cross(f, glm::vec3(0.0f, 1.0f, 0.0f))));
		glm::vec3 const u(cross(s, f));

		glm::mat4 rotation(1);
		rotation[0][0] = s.x;
		rotation[1][0] = s.y;
		rotation[2][0] = s.z;
		rotation[0][1] = u.x;
		rotation[1][1] = u.y;
		rotation[2][1] = u.z;
		rotation[0][2] = f.x;
		rotation[1][2] = f.y;
		rotation[2][2] = f.z;
		rotation[3][0] = 0.0f;
		rotation[3][1] = 0.0f;
		rotation[3][2] = 0.0f;

		rotation = glm::inverse(rotation);

		auto translation = glm::translate(pos + (dir * proj.depth / 2.f));

		auto model = translation * rotation * scale;

		_color_shader->Use();
		_color_shader->SetUniform("Model", model);
		_color_shader->SetUniform("View", state.GetCamera()->GetView());
		_color_shader->SetUniform("Projection", *(state.GetProjectionMatrix()));

		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, _shadow_frustum_node.GetIB().GetNumFaces() * 3, GL_UNSIGNED_INT, nullptr);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		_shadow_frustum_node.GetVAO()->Unbind();
	}
}