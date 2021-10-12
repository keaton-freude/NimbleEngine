#pragma once

#include "glm/glm.hpp"
#include "nimble/core/OrthographicProjection.h"

namespace Nimble {

/**
 * A point light is very similar to a real-world light bulb, though it emits light around itself
 * evenly. A point light has a position,
 */
struct PointLight {
	glm::vec3 position{};
	glm::vec3 ambient{};
	glm::vec3 diffuse{};
	glm::vec3 specular{};

	// The terms of the attenuation function, below are some good starting points for values
	// But they will need modified to fit a particular scene or effect
	/**
	 * Distance	Constant	Linear	Quadratic
		7	1.0	0.7	1.8
		13	1.0	0.35	0.44
		20	1.0	0.22	0.20
		32	1.0	0.14	0.07
		50	1.0	0.09	0.032
		65	1.0	0.07	0.017
		100	1.0	0.045	0.0075
		160	1.0	0.027	0.0028
		200	1.0	0.022	0.0019
		325	1.0	0.014	0.0007
		600	1.0	0.007	0.0002
		3250	1.0	0.0014	0.000007
	 */
	float constant = 1.0f;
	float linear = 0.7f;
	float quadratic = 1.8f;

	bool enabled = false;

	PointLight() = default;
	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, OrthoProjection projection)
	: position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear),
	  quadratic(quadratic), projection(projection) {
	}
};

} // namespace Nimble