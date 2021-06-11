#include "nimble/render-passes/DebugPass.h"
#include <nimble/Mesh.h>
#include <nimble/opengl-wrapper/IndexBuffer.h>
#include <nimble/opengl-wrapper/VertexBuffer.h>
#include <nimble/scene-graph/DirectionalLightNode.h>
#include <nimble/scene-graph/DrawableNode.h>
#include <nimble/utility/ImGuiUtility.h>

using namespace Nimble;

void DebugPass::Draw(SceneState &state, const SceneGraph &sceneGraph) {
	// Variety of optional features which can help in debugging issues
	GUI_CHECKBOX(renderNormals, false);
	GUI_CHECKBOX(showLights, false);
	GUI_CHECKBOX(showShadowFrustrum, false);

	if (showLights) {
		DrawLights(state, sceneGraph);
	}

}
void DebugPass::DrawLights(SceneState &state, const SceneGraph &sceneGraph) {
	// Walk every light, depending on the type draw an indicator in world space
	// Directional lights are represented by a line drawn through the scene
	// Spot lights are represented by a location in world space + direction, etc.

	auto directionalLights = sceneGraph.GetNodesByDerivedType<DirectionalLightNode>(SceneNodeType::DIRECTIONAL_LIGHT);

	for(const auto& light : directionalLights) {
		// Draw an extremely long line along the direction vector of the light
		glm::vec3 pos1 = light->GetDirectionalLight().direction * -20.0f;
		glm::vec3 pos2 = light->GetDirectionalLight().direction * 20.0f;
		std::shared_ptr<VertexArrayObject> vao = std::make_shared<VertexArrayObject>(PositionColor::SetVertexAttribPointers);

		Mesh<PositionColor> mesh(std::vector<PositionColor>{{pos1, {1.0f, 1.0f, 1.0f, 1.0f}}, {pos2, {1.0f, 1.0f, 1.0f, 1.0f}}}, {0, 1}, 1, vao);

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

	for (const auto& light : directionalLights) {

	}

}
