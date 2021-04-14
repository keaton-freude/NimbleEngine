#pragma once

#include "nimble/scene-graph/SceneGraph.h"

namespace Nimble {

class RenderPass {
private:
public:
	virtual void Draw(const SceneGraph& sceneGraph) = 0;
};

}