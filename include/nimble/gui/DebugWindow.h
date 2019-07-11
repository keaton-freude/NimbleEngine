#pragma once

#include "nimble/engine/Engine.h"
#include <memory>

// An Dear ImGui Debug Window for providing debug info in the engine

namespace Nimble {
class DebugWindow {
	std::shared_ptr<Engine> _engine;

public:
	DebugWindow() = delete;
	DebugWindow(std::shared_ptr<Engine> engine) : _engine(engine) {
		// Keep a pointer to the Engine instance to query stats
		// and submit debug commands
	}

	void Draw();
};
} // namespace Nimble