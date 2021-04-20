#pragma once

#include "ShadowPass.h"
#include "nimble/render-passes/RenderPass.h"
#include <nimble/opengl-wrapper/ShaderProgram.h>
#include <nimble/opengl-wrapper/Texture2D.h>

namespace Nimble {

class PhongPass : public RenderPass {
private:
	std::shared_ptr<ShaderProgram> _shader;
	ShadowMap _shadow_map;

public:
	PhongPass();
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
	void SetShadowMap(ShadowMap shadow_map) {
		_shadow_map = shadow_map;
	}
};

} // namespace Nimble
