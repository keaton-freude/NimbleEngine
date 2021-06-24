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
		// return glm::mat4_cast(_rotation) * glm::translate(_position) * glm::scale(_scale);
		// return glm::mat4_cast(_rotation) * glm::translate(_position) * glm::scale(_scale);
		return glm::translate(_position) * glm::mat4_cast(_rotation) * glm::scale(_scale);
		//		glm::mat4 model = glm::mat4(1.0f);
		//		model =
	}

	void Rotate(const glm::quat &rotation) {
		_rotation = rotation * _rotation;
	}

	void SetRotation(const glm::quat &rotation) {
		_rotation = rotation;
	}

	void Translate(glm::vec3 amount) {
		_position += amount;
	}

	void SetTranslation(glm::vec3 translation) {
		_position = translation;
	}

	void Scale(glm::vec3 amount) {
		_scale *= amount;
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

	static Transformation Default() {
		return Transformation{};
	}

	// ---- Operators -----
	Transformation operator*(const Transformation &transformation) {
		Transformation newTransform = *this;
		newTransform._rotation = newTransform._rotation * transformation._rotation;
		newTransform._position = transformation._position + newTransform._position;
		newTransform._scale = transformation._scale * newTransform._scale;

		return newTransform;
	}

	Transformation &operator*=(const Transformation &transformation) {
		_rotation *= transformation._rotation;
		_position += transformation._position;
		_scale *= transformation._scale;

		return *this;
	}

	bool operator==(const Transformation &rhs) const {
		return this->_rotation == rhs._rotation && this->_scale == rhs._scale && this->_position == rhs._position;
	}
};


} // namespace Nimble