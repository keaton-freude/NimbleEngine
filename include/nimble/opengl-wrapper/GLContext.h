#pragma once

#include <GL/glew.h>

#include "nimble/utility/StrongTypes.h"

// Singleton for global OpenGL handles/context/functions

class GLContext {
private:
	GLContext() = default;

public:
	static GLContext &Get() {
		static GLContext _context;
		return _context;
	}

	static void SetViewportDimensions(Width width, Height height) {
		glViewport(0, 0, static_cast<int>(width.get()), static_cast<int>(height.get()));
	}
};
