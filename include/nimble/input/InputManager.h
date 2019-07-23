#pragma once
// Input Singleton, provides per-frame access to keyboard & mouse state
// Only update & provide access to registered Name -> KeyCode mappings

#include <string>
#include <unordered_map>

#include "nimble/utility/Singleton.h"
#include "spdlog/spdlog.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace Nimble {

struct ScanCodeState {
	// What is the scan code?
	int scanCode;
	// is it currently pressed?
	bool pressed;
};

class Input : public Singleton<Input> {
	// Datastructure for storing scan-code state, user identifier -> state
	using ScanCodeStorageTy = std::unordered_map<std::string, ScanCodeState>;

private:
	ScanCodeStorageTy _scanCodeState;
	GLFWwindow *_windowPtr;

	glm::vec2 prevMouse;
	glm::vec2 mouse;

	int rightMouseButtonState = 0;
	int leftMouseButtonState = 0;

	// We only want to capture "did a scroll happen, and if so, in which direction"
	// So -1 or 1 for backwards or forwards
	int scrollWheelXState = 0;
	int scrollWheelYState = 0;

	void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
		spdlog::info("xoffset: {} yoffset: {}", xoffset, yoffset);
		scrollWheelXState = xoffset > 0.0 ? 1 : -1;
		scrollWheelYState = yoffset > 0.0 ? 1 : -1;
	}

public:
	Input() {
		spdlog::info("Input Manager created!");
	}

	void SetGlfwWindow(GLFWwindow *windowPtr) {
		assert(windowPtr);
		_windowPtr = windowPtr;

		// Holy shit GLFW, what a hack.
		glfwSetWindowUserPointer(_windowPtr, (void *)this);

		glfwSetScrollCallback(_windowPtr, [](GLFWwindow *windowPtr, double x, double y) {
			Input *self = static_cast<Input *>(glfwGetWindowUserPointer(windowPtr));
			self->scroll_callback(windowPtr, x, y);
		});

		// glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// Call this method to query the system for new input state
	// Internally we will look at all user-registered mappings, then check their current state
	void Update();

	// Reset the Input manager's internal state
	// Only for things which dont happen every frame like the scroll wheel
	// We can't query the scroll state, so we don't know when to reset it.
	// Probably more things like that.. so fill this function out when needed
	void ClearState() {
		scrollWheelXState = 0;
		scrollWheelYState = 0;
	}

	void RegisterKeyCodeMapping(std::string identifier, int glfwKeyCode);

	bool IsKeyPressed(std::string identifier);

	glm::vec2 GetMouseMovement() {
		return mouse;
	}

	bool IsMouseRightDown() {
		return rightMouseButtonState == GLFW_PRESS;
	}

	bool IsMouseLeftDown() {
		return leftMouseButtonState == GLFW_PRESS;
	}

	bool IsScrollForward() {
		return scrollWheelYState > 0;
	}

	bool IsScrollBackward() {
		return scrollWheelYState < 0;
	}
};
} // namespace Nimble