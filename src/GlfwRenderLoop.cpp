
#include "nimble/GlfwRenderLoop.h"
#include <GLFW/glfw3.h>

using namespace Nimble;

GlfwRenderLoop::GlfwRenderLoop(std::shared_ptr<Engine> engine, GLFWwindow* const windowPointer)
    : RenderLoop(engine, [windowPointer](){
        return glfwWindowShouldClose(windowPointer);
    }), _windowPointer(windowPointer)
{
}

GlfwRenderLoop::~GlfwRenderLoop() {
    glfwTerminate();
}

void GlfwRenderLoop::PollForEvents() {
    glfwPollEvents();
}

void GlfwRenderLoop::SwapBuffers() {
    glfwSwapBuffers(_windowPointer);
}