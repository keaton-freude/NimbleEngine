#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <vector>

#include "nimble/scene-graph/SceneNode.h"

using namespace Nimble;

struct TestState {
	std::vector<int> ids;
};

class TestSceneNode : public SceneNode {
private:
	// For our test, our scene node has an int value
	// As we validate tree functionality, we'll use this value to uniquely
	// identify a scene node
	int _id;
	TestState *_state;

public:
	TestSceneNode(int id, TestState *state) : _id(id), _state(state) {
	}

	void Apply() override {
		_state->ids.push_back(_id);
	}
};

/**
	Tests can utitize this shared state to ensure that traversal, insertion,
	deletion methods, etc are working as expected
	Example: Do a breadth-first search on tree:
			1
		   / \
		  2   3
	ids can be validated that:
	ids[0] == 1, ids[1] == 2, ids[2] == 3
*/

TEST_CASE("SceneNode Add Children", "[scenenode]") {
	TestState state;
	std::unique_ptr<SceneNode> rootNode = std::make_unique<TestSceneNode>(1, &state);
	SceneNode *node1 = new TestSceneNode(2, &state);

	// Add via existing pointer
	rootNode->AddChild(std::unique_ptr<SceneNode>(node1));

	rootNode->Apply();
	node1->Apply();

	REQUIRE(state.ids.size() == 2);
	REQUIRE(state.ids[0] == 1);
	REQUIRE(state.ids[1] == 2);
}

TEST_CASE("SceneGraph Root Traversal")