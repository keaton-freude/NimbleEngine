#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>

namespace Nimble {

struct Character {
	GLuint _textureId;
	// Replace these with vec2i equivalent when we add a vector lib
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};
} // namespace Nimble