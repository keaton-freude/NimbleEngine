#pragma once

// RenderLoop is the base class for the game-engine loop
// It can be used as base class for specific windowing systems (Qt/GLFW/Win32/etc)

#include <chrono>
#include <functional>
#include <memory>

#include "nimble/engine/Engine.h"
#include "nimble/engine/Time.h"

namespace Nimble {

class RenderLoop {
public:
	using ExitCondition = std::function<bool()>;
	// User provides a functor returning a bool which indicates when the loop should end
	RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition);

	virtual ~RenderLoop() = default;

	void Run();

protected:
	virtual void PollForEvents() {
		// default impl we do not care about events.
		// When implementing the render Loop for different systems, override
		// this method to provide platform-specific input gathering
	}

	// Require user to tell us how to swap buffers, as this differs based on platform/system
	virtual void SwapBuffers() = 0;

	void RenderFrame(const Time &time);


private:
	ExitCondition _exitCondition;
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastFrameTime;
	std::shared_ptr<Engine> _engine;
	Time _time;
};

} // namespace Nimble