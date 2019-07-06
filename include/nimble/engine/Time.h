#pragma once

#include <chrono>

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


public:
	Time()
	: _currentTime(std::chrono::steady_clock::now()), _totalFramesCounted(0), _totalTime(0),
	  _framesCounted(0), _fpsTime(0), _lastCalculatedFps(0.0f) {
	}

	// call this as soon as a frame starts
	void Begin() {
		auto newTime = std::chrono::steady_clock::now();
		auto frameTime = newTime - _currentTime;
		_currentTime = newTime;
		_totalTime += frameTime;
		_fpsTime += frameTime;
		if(_fpsTime.count() > 1000000000) {
			_fpsTime -= std::chrono::nanoseconds(1000000000);
			// How many frames did we generate before passing the 1 second mark?
			_lastCalculatedFps = (_framesCounted / 1.0f);
			_framesCounted = 0;
		}
	}

	// call this as soon as a frame ends (after )
	void End() {
		_lastTime = std::chrono::steady_clock::now();
		_framesCounted++;
		_totalFramesCounted++;
	}

	float GetFPS() const {
		return _lastCalculatedFps;
	}
};
} // namespace Nimble