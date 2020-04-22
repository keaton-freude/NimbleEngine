#pragma once

/*
	Represents the current World state. This transformation
	will change as Transformation scene nodes are processed.

	This Transformation can be directly used as the World Matrix
	for shaders.
*/

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "imgui.h"
#include <glm/gtx/transform.hpp>
namespace Nimble {
class Transformation {
private:
	glm::quat _rotation;
	glm::vec3 _position;
	glm::vec3 _scale;

public:
	Transformation() {
		_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		_position = glm::vec3(0.0f, 0.0f, 0.0f);
		_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}


	glm::mat4 GetWorldMatrix() const {
#ifndef NIMBLE_TESTING
#if 0
		ImGui::Text("Transform");
		ImGui::Text("Position: %f, %f, %f", _position.x, _position.y, _position.z);
		ImGui::Text("Rotation: %f, %f, %f %f", _rotation.w, _rotation.x, _rotation.y, _rotation.z);
		ImGui::Text("Scale: %f, %f, %f", _scale.x, _scale.y, _scale.z);
#endif
#endif
		// calculate the world matrix based on the current state
		return glm::mat4_cast(_rotation) * glm::translate(_position) * glm::scale(_scale);
	}

	void Rotate(glm::vec3 axis, float radians) {
		_rotation = glm::angleAxis(radians, axis) * _rotation;
	}

	void SetRotation(glm::vec3 axis, float radians) {
		_rotation = glm::angleAxis(radians, axis);
	}

	void Translate(glm::vec3 amount) {
		_position += amount;
	}

	void Scale(glm::vec3 amount) {
		_scale += amount;
	}
	void SetScale(glm::vec3 scale) {
		_scale = scale;
	}

	glm::quat GetRotation() const {
		return _rotation;
	}

	glm::vec3 GetTranslation() const {
		return _position;
	}

	glm::vec3 GetScale() const {
		return _scale;
	}

	// ---- Operators -----
	Transformation operator*(const Transformation &transformation) {
		Transformation newTransform = *this;
		newTransform._rotation = newTransform._rotation * transformation._rotation;
		newTransform._position = transformation._position + newTransform._position;
		newTransform._scale = transformation._scale * newTransform._scale;

		return newTransform;
	}
};
} // namespace Nimble