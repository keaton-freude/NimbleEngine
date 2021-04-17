#pragma once

#include "nimble/render-passes/RenderPass.h"
#include <nimble/opengl-wrapper/ShaderProgram.h>
#include <nimble/opengl-wrapper/Texture2D.h>

namespace Nimble {

class PhongPass : public RenderPass {
private:
	std::shared_ptr<ShaderProgram> _shader;
	std::shared_ptr<Texture2D> _depth_texture;

public:
	PhongPass();
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
	void SetDepthTexture(std::shared_ptr<Texture2D> depthTexture) {
		_depth_texture = depthTexture;
	}
};

} // namespace Nimble
