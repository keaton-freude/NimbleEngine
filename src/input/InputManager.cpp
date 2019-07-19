#include <GLFW/glfw3.h>

#include "nimble/input/InputManager.h"

using namespace Nimble;
using std::string;
using std::unordered_map;

void Input::Update() {
	// Calculate the mouse delta
	static bool firstRun = true;
	if(firstRun) {
		firstRun = false;
		return;
	}

	double x, y;
	glfwGetCursorPos(_windowPtr, &x, &y);
	glm::vec2 newMouse = glm::vec2(x, y);

	if(newMouse == prevMouse) {
		mouse = glm::vec2(0.0f);
	} else {
		mouse = glm::normalize(newMouse - prevMouse);
	}

	assert(_windowPtr && "Can't check keyboard state if no GLFW window was given!");
	for(auto &[key, value] : _scanCodeState) {
		// Query GLFW for the state of this key and set it
		auto state = glfwGetKey(_windowPtr, value.scanCode);
		value.pressed = state == GLFW_PRESS ? true : false;
	}

	// Query off the mouse state and save it
	prevMouse = newMouse;
}

void Input::RegisterKeyCodeMapping(string identifier, int scanCode) {
	auto scanCodeState = ScanCodeState();
	scanCodeState.pressed = false;
	scanCodeState.scanCode = scanCode;

	_scanCodeState[identifier] = scanCodeState;
}

bool Input::IsKeyPressed(string identifier) {
	auto entry = _scanCodeState.find(identifier);
	if(entry != _scanCodeState.end()) {
		return entry->second.pressed;
	}

	// This string isn't being tracked, will assert for debug, maybe end up removing
	// this if it becomes a pita
	spdlog::debug("InputManager: Identifier {} not registered", identifier);
	assert(false);
}