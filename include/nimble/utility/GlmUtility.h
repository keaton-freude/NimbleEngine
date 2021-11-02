#pragma once

// Provide conversions for non-std types to strings so that catch2 can print them out
// when it needs to
#include <fmt/format.h>
#include <glm/ext.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include <iostream>
std::ostream &operator<<(std::ostream &os, glm::vec3 const &value) {
	os << fmt::format("({}, {}, {})", value.x, value.y, value.z).c_str();
	return os;
}

std::ostream &operator<<(std::ostream &os, glm::quat const &value) {
	os << fmt::format("x: {}, y: {}, z: {}, w: {}", value.x, value.y, value.z, value.w);
	return os;
}

template <typename T>
inline bool glm_equal(const T &a, const T &b) {
	return glm::all(glm::equal(a, b, glm::epsilon<T::value_type>()));
}

template <typename T>
inline bool glm_equal(const T &a, const T &b, float eps) {
	return glm::all(glm::equal(a, b, eps));
}

// Does degrees -> radians conversion which is what GLM tends to use
// for everything. Without this, you must litter `glm::radians` all over
// the place.
inline float operator"" _d(long double d) {
	return (float)glm::radians<long double>(d);
}

inline float operator"" _d(unsigned long long d) {
	return glm::radians((float)d);
}

// Maybe not as useful, but included for completeness
// Does degrees -> radians conversion
inline long double operator"" _r(long double r) {
	return glm::degrees(r);
}
