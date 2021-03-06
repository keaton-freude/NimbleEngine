#pragma once

/**
 *  SceneState encapsulates and groups together the state which is
 *  sent through the scene graph, transformed, and referenced
 *  when rendering drawable objects. It includes the current global
 *  world transform (World Matrix, individual rots/scales/translates)
 *  along with any other set state, like blend mode, shade mode
 *  etc.
 *
 *  Will also include helper pointers which maybe useful, such as a pointer
 *  to the Camera object, and a pointer the current Projection Matrix
 */

#include <glm/glm.hpp>
#include <memory>

#include "nimble/camera/Camera.h"
#include "nimble/light/DirectionalLight.h"
#include "nimble/scene-graph/Transformation.h"

namespace Nimble {
class SceneState {
private:
	std::shared_ptr<glm::mat4> _projectionMatrix;
	std::shared_ptr<ICamera> _camera;
	Transformation _transform;
	DirectionalLight _directionalLight;

public:
	// This class doesn't really make sense without our internal state, lets see
	// if we can get away with deleting the default c'tor
	SceneState() = delete;

	SceneState(std::shared_ptr<glm::mat4> projectionMatrix, std::shared_ptr<ICamera> camera)
	: _projectionMatrix(projectionMatrix), _camera(camera), _transform(), _directionalLight() {
	}

	inline glm::mat4 *GetProjectionMatrix() const {
		return _projectionMatrix.get();
	}

	inline ICamera *GetCamera() const {
		return _camera.get();
	}

	// Users expected to modify this transform, as needed. So support a non-const
	// reference return value for efficient modification
	inline Transformation &GetTransform() {
		return _transform;
	}

	inline DirectionalLight &GetDirectionalLight() {
		return _directionalLight;
	}

	void SetDirectionalLight(const DirectionalLight &light) {
		_directionalLight = light;
		// Make sure we normalize, as our shaders expect it
		_directionalLight.direction = glm::normalize(_directionalLight.direction);
	}
};
} // namespace Nimble