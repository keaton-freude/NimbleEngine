#pragma once

#include "nimble/render-passes/RenderPass.h"
#include "nimble/scene-graph/SceneGraph.h"

namespace Nimble {

class DebugPass : public RenderPass {
private:
	void DrawLights(SceneState& state, const SceneGraph &sceneGraph);
public:
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
};

} // namespace Nimble