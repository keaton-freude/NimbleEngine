#pragma once

#include "nimble/engine/RenderLoop.h"
#include <GLFW/glfw3.h>

namespace Nimble {

class GlfwRenderLoop : public RenderLoop {
public:
	GlfwRenderLoop(std::shared_ptr<Engine> engine, GLFWwindow *const windowPointer);
	~GlfwRenderLoop() override;

protected:
	virtual void PollForEvents() override;
	virtual void SwapBuffers() override;

private:
	GLFWwindow *const _windowPointer;
};

} // namespace Nimble