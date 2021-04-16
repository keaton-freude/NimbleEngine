#pragma once

#include "nimble/render-passes/RenderPass.h"
#include <nimble/opengl-wrapper/ShaderProgram.h>

namespace Nimble {

class PhongPass : public RenderPass {
private:
	std::shared_ptr<ShaderProgram> _shader;

public:
	PhongPass();
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
};

} // namespace Nimble
