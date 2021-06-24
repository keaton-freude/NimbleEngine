#pragma once

#include "nimble/render-passes/RenderPass.h"
#include "nimble/scene-graph/SceneGraph.h"
#include <nimble/IMesh.h>
#include <nimble/scene-graph/DrawableNode.h>

namespace Nimble {

class DebugPass : public RenderPass {
private:
	DrawableNode _shadow_frustum_node;
	std::shared_ptr<ShaderProgram> _color_shader = nullptr;

	// Holds current debug settings state, those of which exposed via GUI
	// TODO: Maybe revisit the ImGui macros at some point to handle one-off flags
	bool _renderNormals = false;
	bool _showLights = false;
	bool _showShadowFrustum = false;
	bool _showAxis = true;

	void DrawLights(SceneState &state, const SceneGraph &sceneGraph);
	void DrawShadowFrustrum(SceneState &state, const SceneGraph &sceneGraph);
	void DrawAxis(SceneState &state, const SceneGraph &sceneGraph);

public:
	DebugPass();
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
};

} // namespace Nimble