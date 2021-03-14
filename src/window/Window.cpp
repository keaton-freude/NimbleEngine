#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GL/glew.h>

#ifdef __linux__
#include <GL/glx.h>
#include <GLFW/glfw3native.h>
#endif
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <stdexcept>

#include "nimble/opengl-wrapper/GLContext.h"
#include "nimble/utility/StrongTypes.h"
#include "nimble/utility/UnusedMacro.h"
#include "nimble/window/Window.h"

using namespace Nimble;

void _HandleResize2(GLFWwindow *window, int width, int height) {
	UNUSED(window);

	GLContext::SetViewportDimensions(Width(static_cast<unsigned>(width)),
									 Height(static_cast<unsigned>(height)));
}

Nimble::Window::Window(Width width, Height height, const char *title)
: _window(nullptr), _height(height), _width(width) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	_window = glfwCreateWindow(static_cast<int>(width.get()), static_cast<int>(height.get()), title,
							   nullptr, nullptr);

	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(!_window) {
		// Failed to create
		throw std::runtime_error(fmt::format("Failed to create window with parameters:"
											 "height: {} width: {} title: {}",
											 height.get(), width.get(), title)
								 .c_str());
	}

	glfwMakeContextCurrent(_window);

#ifdef __linux__
	if (GLX_EXT_swap_control) {
		auto glXSwapIntervalEXT = reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>(glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT"));
		auto display = glfwGetX11Display();
		auto window = glfwGetGLXWindow(_window);
		glXSwapIntervalEXT(display, window, 1);
	} else {
		glfwSwapInterval(1);
	}
#else
	glfwSwapInterval(1);
#endif

	// Setup handler for resize
	glfwSetFramebufferSizeCallback(_window, _HandleResize2);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width.get(), height.get());
	// Fix below for windows
	// glEnable(GL_MULTISAMPLE_ARB);
}

void Nimble::Window::Initialize() const {
	GLContext::SetViewportDimensions(_width, _height);
}
