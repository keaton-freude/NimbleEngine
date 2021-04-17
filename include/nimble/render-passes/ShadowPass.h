#pragma once

#include "RenderPass.h"
#include <memory>
#include <nimble/opengl-wrapper/FrameBuffer.h>
#include <nimble/opengl-wrapper/ShaderProgram.h>
#include <nimble/opengl-wrapper/Texture2D.h>


namespace Nimble {

class ShadowPass : public RenderPass {
private:
	std::shared_ptr<ShaderProgram> _shader;
	std::shared_ptr<Texture2D> _depth_texture;
	FrameBuffer _fbo;
	size_t _shadow_width;
	size_t _shadow_height;

public:
	ShadowPass() = delete;
	ShadowPass(size_t shadowWidth, size_t shadowHeight);

	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;

	std::shared_ptr<Texture2D> GetDepthTexture() {
		return _depth_texture;
	}
};

} // namespace Nimble