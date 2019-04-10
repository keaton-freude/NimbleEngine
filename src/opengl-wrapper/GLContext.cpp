#include <GL/glew.h>

#include "nimble/opengl-wrapper/GLContext.h"

void GLContext::SetViewportDimensions(Width width, Height height) {
	glViewport(0, 0, static_cast<int>(width.get()), static_cast<int>(height.get()));
}
