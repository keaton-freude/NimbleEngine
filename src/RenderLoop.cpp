#include <GL/glew.h>
#include <chrono>
#include <memory>

#include "nimble/RenderLoop.h"
#include "nimble/engine/Engine.h"
#include "nimble/opengl-wrapper/GLContext.h"

#include "spdlog/spdlog.h"

using namespace Nimble;

RenderLoop::RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition)
: _exitCondition(exitCondition), _engine(engine) {
}

void RenderLoop::Run() {
	while(!_exitCondition()) {
		// Poll for Input

		PollForEvents();
		// Render Frame
		RenderFrame();

		SwapBuffers();

		_lastFrameTime = std::chrono::high_resolution_clock::now();
	}
}

void RenderLoop::RenderFrame() {
	_engine->RenderFrame();
}