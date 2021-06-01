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
	unsigned int _model_uniform_location, _view_uniform_location, _projection_uniform_location,
		_uv_multiplier_uniform_location, _view_pos_uniform_location, _lighting_enabled_uniform_location,
		_diffuse_texture_uniform_location, _light_pos_uniform_location, _light_space_matrix_uniform_location,
		_shadow_map_uniform_location;

public:
	PhongPass();
	void Draw(SceneState &state, const SceneGraph &sceneGraph) override;
	void SetShadowMap(ShadowMap shadow_map) {
		_shadow_map = shadow_map;
	}
};

} // namespace Nimble
