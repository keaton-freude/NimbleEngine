#include <GL/glew.h>
#include <chrono>
#include <memory>

#include "nimble/engine/Engine.h"
#include "nimble/engine/RenderLoop.h"
#include "nimble/engine/Time.h"
#include "nimble/opengl-wrapper/GLContext.h"

#include "spdlog/spdlog.h"

using namespace Nimble;

RenderLoop::RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition)
: _exitCondition(exitCondition), _engine(engine) {
}

void RenderLoop::Run() {
	while(!_exitCondition()) {
		_time.Begin();

		// Poll for Input
		PollForEvents();

		// Render Frame
		RenderFrame(_time);

		SwapBuffers();

		_time.End();
	}
}

void RenderLoop::RenderFrame(const Time &time) {
	_engine->RenderFrame(time);
}