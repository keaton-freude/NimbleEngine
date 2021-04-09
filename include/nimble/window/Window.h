#pragma once
#include "GLFW/glfw3.h"
#include "nimble/utility/StrongTypes.h"

namespace Nimble {
class Window {
private:
	// GLFW owns the pointer, no need for delete
	GLFWwindow *_window;
	Height _height;
	Width _width;

public:
	explicit Window(Width width, Height height, const char *title);

	GLFWwindow *GetWindow() const {
		return _window;
	}
	void Initialize() const;

	void SetVSync(bool enabled);

private:
};

void _HandleResize(GLFWwindow *window, int width, int height);
}