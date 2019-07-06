
#include "nimble/engine/GlfwRenderLoop.h"
#include "nimble/input/InputManager.h"
#include <GLFW/glfw3.h>

using namespace Nimble;

GlfwRenderLoop::GlfwRenderLoop(std::shared_ptr<Engine> engine, GLFWwindow *const windowPointer)
: RenderLoop(engine, [windowPointer]() { return glfwWindowShouldClose(windowPointer); }),
  _windowPointer(windowPointer) {
}

GlfwRenderLoop::~GlfwRenderLoop() {
	glfwTerminate();
}

void GlfwRenderLoop::PollForEvents() {
	glfwPollEvents();
	Input::Get().Update();

	// Typically we shouldn't do input lookups in the render loop
	// but, checking for the event which is going to end the render loop
	// seems to be a fair exception
	if(Input::Get().IsKeyPressed("exit")) {
		glfwSetWindowShouldClose(_windowPointer, 1);
	}
}

void GlfwRenderLoop::SwapBuffers() {
	glfwSwapBuffers(_windowPointer);
}