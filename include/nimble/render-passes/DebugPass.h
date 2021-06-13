#pragma once

#include "nimble/render-passes/RenderPass.h"
#include "nimble/scene-graph/SceneGraph.h"
#include <nimble/IMesh.h>
#include <nimble/scene-graph/DrawableNode.h>

namespace Nimble {

class DebugPass : public RenderPass {
private:
	DrawableNode _shadow_frustum_node;
	VertexArrayObject _shadow_frustum_vao;

	void DrawLights(SceneState &state, const SceneGraph &sceneGraph);
	void DrawShadowFrustrum(SceneState &state, const SceneGraph &sceneGraph);

public:
	DebugPass();
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
};

} // namespace Nimble