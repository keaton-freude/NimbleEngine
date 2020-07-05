#pragma once

/**
 * `Time` is a class which provides time-related functionality related to runtime
 * time statistics. Providing `dt` (the time between drawn frames), time statistics
 * such as historical frametime statistics and more.
 *
 * `Time` _should not_ have any side effects on actual program execution.
 */

#include <EASTL/bonus/ring_buffer.h>
#include <chrono>
#include <vector>

#include "EASTL/fixed_vector.h"
#include "implot.h"
#include "spdlog/spdlog.h"

namespace Nimble {

class Time {
	std::chrono::time_point<std::chrono::steady_clock> _lastTime;
	std::chrono::time_point<std::chrono::steady_clock> _currentTime;

	// All frames ever rendered
	uint64_t _totalFramesCounted;

	// Total time
	std::chrono::nanoseconds _totalTime;

	// How many frames have been counted in the last second?
	uint32_t _framesCounted;

	// Keeps track of frame time until its greater than 1.0, then we'll wrap around
	// and spit out the new FPS value
	std::chrono::nanoseconds _fpsTime;

	// Last calculated FPS value
	float _lastCalculatedFps;

	// Delta time between renders
	std::chrono::nanoseconds _frameTime;

public:
	Time()
	: _currentTime(std::chrono::steady_clock::now()), _totalFramesCounted(0), _totalTime(0),
	  _framesCounted(0), _fpsTime(0), _lastCalculatedFps(0.0f), _frameTime(0) {
	}

	// call this as soon as a frame starts
	void Begin() {
		auto newTime = std::chrono::steady_clock::now();
		_frameTime = newTime - _currentTime;
		_currentTime = newTime;
		_totalTime += _frameTime;
		_fpsTime += _frameTime;

		if(_fpsTime.count() > 1'000'000'000) {
			_fpsTime -= std::chrono::nanoseconds(1'000'000'000);
			// How many frames did we generate before passing the 1 second mark?
			_lastCalculatedFps = (_framesCounted / 1.0f);
			_framesCounted = 0;
		}
	}

	// call this as soon as a frame ends
	void End() {
		_lastTime = std::chrono::steady_clock::now();
		_framesCounted++;
		_totalFramesCounted++;
	}

	[[nodiscard]] float GetFPS() const {
		return _lastCalculatedFps;
	}

	[[nodiscard]] float dt() const {
		return std::chrono::duration_cast<std::chrono::duration<float>>(_frameTime).count();
	}

	[[nodiscard]] float GetFrameTime() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(_frameTime).count();
	}
};
} // namespace Nimble