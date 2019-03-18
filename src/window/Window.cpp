#include "nimble/window/Window.h"
#include "fmt/format.h"
#include "nimble/opengl-wrapper/GLContext.h"
#include "nimble/utility/UnusedMacro.h"
#include <exception>

void _HandleResize2(GLFWwindow *window, int width, int height) {
	UNUSED(window);

	GLContext::SetViewportDimensions(Width(width), Height(height));
}

Window::Window(Width width, Height height, const char *title)
: _window(nullptr), _height(height), _width(width) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(width.get(), height.get(), title, nullptr, nullptr);

	if(!_window) {
		// Failed to create
		throw std::exception(fmt::format("Failed to create window with parameters:"
										 "height: {} width: {} title: {}",
										 height.get(), width.get(), title)
							 .c_str());
	}

	glfwMakeContextCurrent(_window);

	// Setup handler for resize
	glfwSetFramebufferSizeCallback(_window, _HandleResize2);
}

void Window::Initialize() const {
	GLContext::SetViewportDimensions(_width, _height);
}
