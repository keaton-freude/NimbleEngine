#pragma once
// Input Singleton, provides per-frame access to keyboard & mouse state
// Only update & provide access to registered Name -> KeyCode mappings

#include <string>
#include <unordered_map>

#include "nimble/utility/Singleton.h"
#include "spdlog/spdlog.h"

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

public:
	Input() {
		spdlog::info("Input Manager created!");
	}

	void SetGlfwWindow(GLFWwindow *windowPtr) {
		assert(windowPtr);
		_windowPtr = windowPtr;
	}

	// Call this method to query the system for new input state
	// Internally we will look at all user-registered mappings, then check their current state
	void Update();

	void RegisterKeyCodeMapping(std::string identifier, int glfwKeyCode);

	bool IsKeyPressed(std::string identifier);
};
} // namespace Nimble