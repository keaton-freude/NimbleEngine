#pragma once

#include "nimble/render-passes/RenderPass.h"

namespace Nimble {

class PhongPass : public RenderPass {
private:
public:
	void Draw(const SceneGraph& sceneGraph) override;
};

}
