#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

#include "nimble/opengl-wrapper/GLContext.h"
#include "nimble/utility/StrongTypes.h"
#include "nimble/utility/UnusedMacro.h"
#include "nimble/window/Window.h"

void _HandleResize2(GLFWwindow *window, int width, int height) {
	UNUSED(window);

	GLContext::SetViewportDimensions(Width(static_cast<unsigned>(width)),
									 Height(static_cast<unsigned>(height)));
}

Window::Window(Width width, Height height, const char *title)
: _window(nullptr), _height(height), _width(width) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	_window = glfwCreateWindow(static_cast<int>(width.get()), static_cast<int>(height.get()), title,
							   nullptr, nullptr);

	if(!_window) {
		// Failed to create
		throw std::runtime_error(fmt::format("Failed to create window with parameters:"
											 "height: {} width: {} title: {}",
											 height.get(), width.get(), title)
								 .c_str());
	}

	glfwMakeContextCurrent(_window);

	// uncomment below line to disable vsync
	//	glfwSwapInterval(0);

	// Setup handler for resize
	glfwSetFramebufferSizeCallback(_window, _HandleResize2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void Window::Initialize() const {
	GLContext::SetViewportDimensions(_width, _height);
}
