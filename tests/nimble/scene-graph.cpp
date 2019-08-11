#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#define NIMBLE_TESTING

#include <vector>

#include <glm/gtc/epsilon.hpp>

#include "nimble/scene-graph/SceneGraph.h"
#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/Transformation.h"

using namespace Nimble;

struct TestState {
	std::vector<int> values;
};

class TestSceneNode : public SceneNode {
private:
	// For our test, our scene node has an int value
	// As we validate tree functionality, we'll use this value to uniquely
	// identify a scene node
	int _value;
	TestState *_state;

public:
	TestSceneNode(int value, TestState *state) : _value(value), _state(state) {
	}

	void Apply(SceneState &sceneState) override {
		_state->values.push_back(_value);
	}
};

/**
	Tests can utitize this shared state to ensure that traversal, insertion,
	deletion methods, etc are working as expected
	Example: Do a breadth-first search on tree:
			1
		   / \
		  2   3
	values can be validated that:
	values[0] == 1, values[1] == 2, values[2] == 3
*/

TEST_CASE("SceneNode Add Children", "[scenenode]") {
	TestState state;
	std::unique_ptr<SceneNode> rootNode = std::make_unique<TestSceneNode>(1, &state);
	SceneNode *node1 = new TestSceneNode(2, &state);
	SceneState fakeState(nullptr, nullptr);

	// Add via existing pointer
	rootNode->AddChild(node1);

	rootNode->Apply(fakeState);
	node1->Apply(fakeState);

	REQUIRE(state.values.size() == 2);
	REQUIRE(state.values[0] == 1);
	REQUIRE(state.values[1] == 2);
}

TEST_CASE("Get SceneNode ID", "[scenenode]") {
	// Scene nodes have an ID which is unique across all created Scene Nodes

	// Need some derived SceneNode, even though not using state tracker, will just use the
	// class for convenience
	TestState state;
	auto node = std::make_unique<TestSceneNode>(1, &state);
	auto node2 = std::make_unique<TestSceneNode>(1, &state);


	auto id = node->GetID();

	// TODO: I know, because I wrote the impl that the first ID returned is going to be 0
	// How can I change this test to be more resilient? Or is this fine..

	REQUIRE(node2->GetID() == id + 1);
}

TEST_CASE("Traversal", "[scenenode]") {
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
	SceneState fakeState(nullptr, nullptr);

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

	root->Visit(fakeState);

	REQUIRE(state.values.size() == 9);
	REQUIRE(state.values[0] == 1);
	REQUIRE(state.values[1] == 2);
	REQUIRE(state.values[2] == 4);
	REQUIRE(state.values[3] == 5);
	REQUIRE(state.values[4] == 3);
	REQUIRE(state.values[5] == 6);
	REQUIRE(state.values[6] == 7);
	REQUIRE(state.values[7] == 8);
	REQUIRE(state.values[8] == 9);
}

TEST_CASE("Find nodes", "[scenenode]") {
	TestState state{};
	SceneNode *root = new TestSceneNode(1, &state);

	SceneNode *node1 = new TestSceneNode(1, &state);
	SceneNode *node2 = new TestSceneNode(1, &state);
	SceneNode *node3 = new TestSceneNode(1, &state);
	SceneNode *node4 = new TestSceneNode(1, &state);
	SceneNode *node5 = new TestSceneNode(1, &state);

	auto node1Id = node1->GetID();
	auto node5Id = node5->GetID();

	root->AddChild(node1);
	root->AddChild(node2);

	node1->AddChild(node3);
	node2->AddChild(node4);
	node4->AddChild(node5);

	auto &found1 = root->Find(node1Id);
	REQUIRE(found1.has_value());
	REQUIRE(found1.value()->GetID() == node1Id);

	auto &found2 = root->Find(node5Id);

	REQUIRE(found2.has_value());
	REQUIRE(found2.value()->GetID() == node5Id);
}

// Somewhat different in that we will create a scene graph with a few nodes, construct two different
// node hierarchys. Then we will add these node hierarchies to 2 different points based on IDs
/*

						View of Test Graph
						------------------

								0
							   / \
							  1   2
							 /   /
							3   7
						   / \   \
						  4   5   8
							   \   \
								6   9

	So we start by adding node 1 & 2 as children to the graphs root node
	Then, we construct 2 new parent nodes (in our user code), attaching nodes 4, 5, 6 to one, and
	7,8,9 to the other

	Then we will add these 2 parent nodes directly to node #1 & node #2, creating the above structure.
*/
TEST_CASE("Add Nodes to SceneGraph", "[scenegraph]") {
	TestState state{};
	SceneGraph graph(nullptr, nullptr);

	// Root node is automatically created by the SceneGraph
	SceneNode *node1 = new TestSceneNode(1, &state);
	SceneNode *node2 = new TestSceneNode(2, &state);

	// Add some new root children, which is a special operation in the scene graph which
	// inserts nodes agaginst the root node in the graph
	auto node1Id = graph.AddChildToRoot(node1);
	auto node2Id = graph.AddChildToRoot(node2);

	SceneNode *node3 = new TestSceneNode(3, &state);
	SceneNode *node4 = new TestSceneNode(4, &state);
	SceneNode *node5 = new TestSceneNode(5, &state);
	SceneNode *node6 = new TestSceneNode(6, &state);

	node3->AddChild(node4);
	node3->AddChild(node5);
	node5->AddChild(node6);

	SceneNode *node7 = new TestSceneNode(7, &state);
	SceneNode *node8 = new TestSceneNode(8, &state);
	SceneNode *node9 = new TestSceneNode(9, &state);

	node7->AddChild(node8);
	node8->AddChild(node9);

	// Now we can add our constructed sub-graphs into the overall graph
	graph.AddChild(node3, node1Id);
	graph.AddChild(node7, node2Id);

	graph.Render();

	REQUIRE(state.values.size() == 9);
	REQUIRE(state.values[0] == 1);
	REQUIRE(state.values[1] == 3);
	REQUIRE(state.values[2] == 4);
	REQUIRE(state.values[3] == 5);
	REQUIRE(state.values[4] == 6);
	REQUIRE(state.values[5] == 2);
	REQUIRE(state.values[6] == 7);
	REQUIRE(state.values[7] == 8);
	REQUIRE(state.values[8] == 9);
}

TEST_CASE("Apply Transformations", "[transform]") {
	// Ensure we can multiple Transforms together in order to apply their attributes

	Transformation transform1;
	Transformation transform2;

	// Rotate along the x-axis 90 degrees...
	transform1.SetRotation(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(90.0f));

	// Rotate the other along the y-axis 90 degrees..
	transform2.SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f));

	// Create a new transform from the result of applying transform2 to transform1
	// Which should result in a rotation which is 90 degrees around the x-axis, and 90 degrees around the y-axis
	Transformation combinedTransform = transform1 * transform2;

	REQUIRE(glm::epsilonEqual(combinedTransform.GetRotation().w, 0.5f, glm::epsilon<float>()));
}