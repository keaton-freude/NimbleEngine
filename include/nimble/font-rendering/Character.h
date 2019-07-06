#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>

namespace Nimble {

struct Character {
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};
} // namespace Nimble