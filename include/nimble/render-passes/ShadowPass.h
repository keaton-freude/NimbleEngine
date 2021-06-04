#pragma once

#include "RenderPass.h"
#include <memory>
#include <nimble/opengl-wrapper/FrameBuffer.h>
#include <nimble/opengl-wrapper/ShaderProgram.h>
#include <nimble/opengl-wrapper/Texture2D.h>


namespace Nimble {

struct ShadowMap {
	std::shared_ptr<Texture2D> depth_texture;
	glm::mat4 light_space_matrix;
};

class ShadowPass : public RenderPass {
private:
	std::shared_ptr<ShaderProgram> _shader;
	ShadowMap _shadow_map;
	FrameBuffer _fbo;
	int _shadow_width;
	int _shadow_height;

public:
	ShadowPass() = delete;
	ShadowPass(int shadowWidth, int shadowHeight);

	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;

	ShadowMap GetShadowMap() {
		return _shadow_map;
	}
};

} // namespace Nimble