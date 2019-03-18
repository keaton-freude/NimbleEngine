#pragma once

namespace Nimble {
struct Position {
	enum { SizeInBytes = sizeof(float) * 3 };
	float x;
	float y;
	float z;

	Position() = default;
	Position(float x, float y, float z) : x(x), y(y), z(z) {
	}
};
} // namespace Nimble