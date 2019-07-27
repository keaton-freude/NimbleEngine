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

TEST_CASE("Traversal", "[scenegraph]") {
	// Test that Traversal works correctly. The SceneGraph supports Pre-order
	// traversal, which means that we will visit nodes in the following order:
	// <root>,<left>,<right>
	/*
				View of Test Graph
						1
					   / \
					  2   3
					 /\     \
					4  5     6
							  \
							   7
							  / \
							 8   9

		The expected output is:
		1,2,4,5,3,6,7,8,9

		We may end up adding more traversal methods, or changing this one. For now
		we will see how far we can get with just pre-order traversal */

	TestState state{};

	// Raw pointers because lazy
	SceneNode *root = new TestSceneNode(1, &state);


	SceneNode *node2 = new TestSceneNode(2, &state);
	SceneNode *node3 = new TestSceneNode(3, &state);
	SceneNode *node4 = new TestSceneNode(4, &state);
	SceneNode *node5 = new TestSceneNode(5, &state);
	SceneNode *node6 = new TestSceneNode(6, &state);
	SceneNode *node7 = new TestSceneNode(7, &state);
	SceneNode *node8 = new TestSceneNode(8, &state);
	SceneNode *node9 = new TestSceneNode(9, &state);

	// Wire up the children
	root->AddChild(node2);
	root->AddChild(node3);

	node2->AddChild(node4);
	node2->AddChild(node5);

	node3->AddChild(node6);
	node6->AddChild(node7);

	node7->AddChild(node8);
	node7->AddChild(node9);

	root->Visit();

	REQUIRE(state.ids.size() == 9);
	REQUIRE(state.ids[0] == 1);
	REQUIRE(state.ids[1] == 2);
	REQUIRE(state.ids[2] == 4);
	REQUIRE(state.ids[3] == 5);
	REQUIRE(state.ids[4] == 3);
	REQUIRE(state.ids[5] == 6);
	REQUIRE(state.ids[6] == 7);
	REQUIRE(state.ids[7] == 8);
	REQUIRE(state.ids[8] == 9);
}