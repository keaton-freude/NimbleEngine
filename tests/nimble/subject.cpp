#include <catch2/catch.hpp>

#define NIMBLE_TESTING

#include "nimble/core/Subject.h"

using namespace Nimble;

TEST_CASE("Subject Emit", "[subject]") {
	Subject<int> subject;
	int out = 0;
	subject.Subscribe([&out](int value) { out = value; });

	subject.Emit(42);

	REQUIRE(out == 42);
}