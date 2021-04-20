#include "nimble/render-passes/DebugPass.h"

using namespace Nimble;

void DebugPass::Draw(SceneState &state, const SceneGraph &sceneGraph) {
	// Variety of optional features which can help in debugging issues
	static bool render_normals = false;
	ImGui::Checkbox("Normals", &render_normals);
	static bool show_lights = false;
	ImGui::Checkbox("Show Lights", &show_lights);
}