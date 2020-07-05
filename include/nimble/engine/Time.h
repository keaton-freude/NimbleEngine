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

namespace Nimble {

struct FrameStats {
	float maxFrametime;
	float minFrametime;

	void Reset() {
		maxFrametime = 0.0f;
		minFrametime = 0.0f;
	}
};

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

	// Ring buffer for maximum frametimes over specified timeslice
	static constexpr std::chrono::nanoseconds _timesliceLength = std::chrono::nanoseconds(100'000'000);
	static constexpr uint16_t _maxFrameTimeHistorySize = 50;

	using FrameTimeHistoryContainer = eastl::fixed_vector<double, _maxFrameTimeHistorySize>;

	// Current setup will retain 25 seconds worth of data, sampled every 0.5s
	FrameTimeHistoryContainer _maxFrameTimeHistory;
	FrameTimeHistoryContainer::iterator _maxFrameTimeHistoryIter;
	size_t index = 0;

	// Keep track of our sampling frequency separate from the fpsTime so we can exceed 1.0s
	std::chrono::nanoseconds _frameHistoryTime = std::chrono::nanoseconds(0);

	double _currentMaxFrameTime = -1.0f;

	void SampleFrametimeHistory() {
		using namespace std::chrono;
		const double frameTimeMs = duration_cast<milliseconds>(_frameTime).count() / 1000.0;
		//double frameTimeMs = rand() * 100;
		_currentMaxFrameTime = eastl::max(_currentMaxFrameTime, frameTimeMs);

		_frameHistoryTime += _frameTime;
		// Check if we've passed our threshold

		if(_frameHistoryTime >= _timesliceLength) {
			_frameHistoryTime -= _timesliceLength;

			// sample the time
			//_maxFrameTimeHistory.push_back(_currentMaxFrameTime);
			_maxFrameTimeHistory[index++ % _maxFrameTimeHistorySize] = _currentMaxFrameTime;
			_currentMaxFrameTime = -1.0;

			if(_maxFrameTimeHistory.size() > _maxFrameTimeHistorySize) {
					_maxFrameTimeHistoryIter++;
				if (_maxFrameTimeHistoryIter = _maxFrameTimeHistory.end()) {
					_maxFrameTimeHistoryIter = _maxFrameTimeHistory.begin();
				}
			} else {
				_maxFrameTimeHistoryIter = _maxFrameTimeHistory.begin();
			}
		}
	}

public:
	Time()
	: _currentTime(std::chrono::steady_clock::now()), _totalFramesCounted(0), _totalTime(0),
	  _framesCounted(0), _fpsTime(0), _lastCalculatedFps(0.0f), _frameTime(0),
	  _maxFrameTimeHistory(_maxFrameTimeHistorySize) {
		_maxFrameTimeHistoryIter = _maxFrameTimeHistory.begin();
	}

	// call this as soon as a frame starts
	void Begin() {
		auto newTime = std::chrono::steady_clock::now();
		_frameTime = newTime - _currentTime;
		_currentTime = newTime;
		_totalTime += _frameTime;
		_fpsTime += _frameTime;

		SampleFrametimeHistory();


		if(_fpsTime.count() > 1'000'000'000) {
			_fpsTime -= std::chrono::nanoseconds(1'000'000'000);
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

	[[nodiscard]] float GetFPS() const {
		return _lastCalculatedFps;
	}

	[[nodiscard]] float dt() const {
		return std::chrono::duration_cast<std::chrono::duration<float>>(_frameTime).count();
	}

	[[nodiscard]] float GetFrameTime() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(_frameTime).count();
	}

	// Iterator to the "beginning" of the ring_buffer with respect to frametime history
	[[nodiscard]] FrameTimeHistoryContainer::iterator GetFrameTimeHistoryIterator() const {
		return _maxFrameTimeHistoryIter;
	}

	[[nodiscard]] size_t GetNumHistoryFrames() const {
		return _maxFrameTimeHistory.size();
	}

	[[nodiscard]] double GetMaxFrameTimeHistory() const {
		double max = -1.0;
		for(const auto &time : _maxFrameTimeHistory) {
			max = eastl::max(max, time);
		}
		return max;
	}
};
} // namespace Nimble