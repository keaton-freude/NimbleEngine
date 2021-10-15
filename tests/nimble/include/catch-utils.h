#pragma once

// Provide conversions for non-std types to strings so that catch2 can print them out
// when it needs to

#include <fmt/format.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <iostream>
std::ostream &operator<<(std::ostream &os, glm::vec3 const &value) {
	os << fmt::format("({}, {}, {})", value.x, value.y, value.z).c_str();
	return os;
}


// The catch2 include must be after any of the provided ostream overloads
#include <catch2/catch.hpp>
#include <glm/ext/scalar_constants.hpp>

template <typename T>
inline bool glm_equal(const T &a, const T &b) {
	return glm::all(glm::equal(a, b, 0.000001f));
}
