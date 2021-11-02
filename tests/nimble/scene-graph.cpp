#define CATCH_CONFIG_MAIN

#define NIMBLE_TESTING

#include "nimble/scene-graph/SceneGraph.h"
#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/Transformation.h"
#include "nimble/utility/GlmUtility.h"
#include <glm/gtc/epsilon.hpp>
#include <vector>

// The catch2 include must be after any of the provided ostream overloads
// which come from GlmUtility.h
#include <catch2/catch.hpp>

using namespace Nimble;

// a lax vec3 comparison, rotation rollbacks are lossy
// which is fine for the purposes of the engine as the differences
// are not noticeable
bool operator==(const glm::vec3 &a, const glm::vec3 &b) {
	static constexpr float EPS = 0.00001f;
	return glm::all(glm::equal(a, b, EPS));
}

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
	TestSceneNode(int value, TestState *state) : SceneNode("test_node"), _value(value), _state(state) {
	}

	void Apply(SceneState &sceneState) override {
		_state->values.push_back(_value);
	}

	void DrawGuiElements() override {
	}

	SceneNodeType GetSceneNodeType() override {
		return SceneNodeType::UNKNOWN;
	}

	[[nodiscard]] int GetValue() const {
		return _value;
	}
};

class TestSceneNode2 : public TestSceneNode {
public:
	TestSceneNode2(int value, TestState *state) : TestSceneNode(value, state) {
	}

	void Apply(SceneState &sceneState) override {
		//
	}

	SceneNodeType GetSceneNodeType() override {
		return SceneNodeType::DIRECTIONAL_LIGHT;
	}
};

/**
	Tests can utilize this shared state to ensure that traversal, insertion,
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

//		We may end up adding more traversal methods, or changing this one. For now
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

	auto found1 = root->Find(node1Id);
	REQUIRE(found1.has_value());
	REQUIRE(found1.value()->GetID() == node1Id);

	auto found2 = root->Find(node5Id);

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
	// inserts nodes against the root node in the graph
	auto node1Id = graph.AddChildToRoot(node1).second;
	auto node2Id = graph.AddChildToRoot(node2).second;

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
	// Ensure we can multiply Transforms together in order to apply their attributes

	Transformation transform1;
	Transformation transform2;

	// Rotate along the x-axis 90 degrees...
	transform1.SetRotation(glm::angleAxis(90.0_d, glm::vec3(1.0f, 0.0f, 0.0f)));

	// Rotate the other along the y-axis 90 degrees..
	transform2.SetRotation(glm::angleAxis(90.0_d, glm::vec3(0.0f, 1.0f, 0.0f)));

	// Create a new transform from the result of applying transform2 to transform1
	// Which should result in a rotation which is 90 degrees around the x-axis, and 90 degrees around the y-axis
	Transformation combinedTransform = transform1 * transform2;

	REQUIRE(glm::epsilonEqual(combinedTransform.GetRotation().w, 0.5f, glm::epsilon<float>()));
}

TEST_CASE("Apply Transformations to Scene Nodes", "[scenegraph]") {
	std::unique_ptr<SceneGraph> sceneGraph = std::make_unique<SceneGraph>(nullptr, nullptr);
	TestState state{};

	// Build up the scene graph, using default transforms for each node. Then modify
	// transforms throughout the graph ensuring transform changes are propagated
	auto *testNode1 = new TestSceneNode(1, &state);
	auto *testNode2 = new TestSceneNode(2, &state);

	auto *testNode3 = new TestSceneNode(3, &state);
	auto *testNode4 = new TestSceneNode(4, &state);
	auto *testNode5 = new TestSceneNode(5, &state);
	auto *testNode6 = new TestSceneNode(6, &state);

	/*
	 *                1
	 *               / \
	 *              2   3
	 *                 / \
	 *                6   4
	 *                     \
	 *                      5
	 */

	testNode1->AddChild(testNode2);

	testNode1->AddChild(testNode3).first->AddChild(testNode4).first->AddChild(testNode5);
	testNode3->AddChild(testNode6);

	// Check that we're starting with default transforms on all entries
	REQUIRE(testNode1->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(testNode2->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(testNode3->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(testNode4->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(testNode5->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(testNode6->GetGlobalTransformation() == Transformation::Default());

	// Modify root...
	testNode1->Translate(glm::vec3(10.0f, 0.0f, 0.0f));

	// Every transform should now be translated +10.0f along x axis
	Transformation expectedTransform1 = Transformation::Default();
	expectedTransform1.Translate(glm::vec3(10.0f, 0.0f, 0.0f));

	REQUIRE(testNode1->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(testNode2->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(testNode3->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(testNode4->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(testNode5->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(testNode6->GetGlobalTransformation() == expectedTransform1);

	// Now transform a node lower in the hierarchy
	testNode3->Translate(glm::vec3(5.0f, 10.0f, 0.0f));
	Transformation expectedTransform2 = expectedTransform1;
	expectedTransform2.Translate(glm::vec3(5.0f, 10.0f, 0.0f));

	// Should not be modified
	REQUIRE(testNode1->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(testNode2->GetGlobalTransformation() == expectedTransform1);

	// Should be modified
	REQUIRE(testNode3->GetGlobalTransformation() == expectedTransform2);
	REQUIRE(testNode4->GetGlobalTransformation() == expectedTransform2);
	REQUIRE(testNode5->GetGlobalTransformation() == expectedTransform2);
	REQUIRE(testNode6->GetGlobalTransformation() == expectedTransform2);

	testNode1->Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	testNode1->Rotate(glm::angleAxis(45.0_d, glm::vec3(1.0f, 1.0f, 0.0f)));
	testNode1->Translate(glm::vec3(10.0f, 10.0f, 10.0f));

	Transformation expectedTransform3 = expectedTransform1;
	Transformation expectedTransform4 = expectedTransform2;

	expectedTransform3.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	expectedTransform3.Rotate(glm::angleAxis(45.0_d, glm::vec3(1.0f, 1.0f, 0.0f)));
	expectedTransform3.Translate(glm::vec3(10.0f, 10.0f, 10.0f));

	expectedTransform4.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	expectedTransform4.Rotate(glm::angleAxis(45.0_d, glm::vec3(1.0f, 1.0f, 0.f)));
	expectedTransform4.Translate(glm::vec3(10.0f, 10.0f, 10.0f));

	REQUIRE(testNode1->GetGlobalTransformation() == expectedTransform3);
	REQUIRE(testNode2->GetGlobalTransformation() == expectedTransform3);

	// Should be modified
	REQUIRE(testNode3->GetGlobalTransformation() == expectedTransform4);
	REQUIRE(testNode4->GetGlobalTransformation() == expectedTransform4);
	REQUIRE(testNode5->GetGlobalTransformation() == expectedTransform4);
	REQUIRE(testNode6->GetGlobalTransformation() == expectedTransform4);
}

TEST_CASE("Apply Transformations to Scene Graph Edge Cases", "[scenegraph]") {
	std::unique_ptr<SceneGraph> sceneGraph = std::make_unique<SceneGraph>(nullptr, nullptr);
	TestState state{};

	/*
	 * 		Scene Graph state
	 *
	 *                 1
	 *                / \
	 *               2   3
	 *                  / \
	 *                 4   5
	 *
	 */

	SceneNode *sceneNode1 = new TestSceneNode(1, &state);

	SceneNode *sceneNode2 = new TestSceneNode(2, &state);
	SceneNode *sceneNode3 = new TestSceneNode(3, &state);
	SceneNode *sceneNode4 = new TestSceneNode(4, &state);
	SceneNode *sceneNode5 = new TestSceneNode(5, &state);

	sceneNode1->AddChild(sceneNode2);
	sceneNode1->AddChild(sceneNode3).first->AddChild(sceneNode4);
	sceneNode3->AddChild(sceneNode5);

	// Ensure modifying a leaf node doesn't modify any other node
	sceneNode2->Translate(glm::vec3(10.0f, 0.0f, 0.0f));
	Transformation expectedTransform1{};
	expectedTransform1.Translate(glm::vec3(10.0f, 0.0f, 0.0f));

	// Ensure the nodes transform has changed..
	REQUIRE(sceneNode2->GetGlobalTransformation() == expectedTransform1);


	// .. and the rest have not
	REQUIRE(sceneNode1->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(sceneNode3->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(sceneNode4->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(sceneNode5->GetGlobalTransformation() == Transformation::Default());

	sceneNode2->Translate(glm::vec3(-10.0f, 0.0f, 0.0f));
	expectedTransform1.Translate(glm::vec3(-10.0f, 0.0f, 0.0f));

	// Try another leaf node, this one further down the hierarchy
	sceneNode3->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
	expectedTransform1.Translate(glm::vec3(0.0f, 5.0f, 0.0f));

	REQUIRE(sceneNode3->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(sceneNode4->GetGlobalTransformation() == expectedTransform1);
	REQUIRE(sceneNode5->GetGlobalTransformation() == expectedTransform1);

	REQUIRE(sceneNode1->GetGlobalTransformation() == Transformation::Default());
	REQUIRE(sceneNode2->GetGlobalTransformation() == Transformation::Default());
}

TEST_CASE("Add Children", "[scenegraph]") {
	std::unique_ptr<SceneGraph> sceneGraph = std::make_unique<SceneGraph>(nullptr, nullptr);
	TestState state{};
	sceneGraph->AddChildrenToRoot(new TestSceneNode(1, &state), new TestSceneNode(2, &state));

	sceneGraph->Render();

	REQUIRE(state.values.size() == 2);
	REQUIRE(state.values[0] == 1);
	REQUIRE(state.values[1] == 2);

	state.values.clear();

	SceneNode *node3 = new TestSceneNode(3, &state);

	sceneGraph->AddChildToRoot(node3);

	node3->AddChildren(new TestSceneNode(4, &state), new TestSceneNode(5, &state), new TestSceneNode(6, &state));

	sceneGraph->Render();

	REQUIRE(state.values.size() == 6);
	REQUIRE(state.values[0] == 1);
	REQUIRE(state.values[1] == 2);
	REQUIRE(state.values[2] == 3);
	REQUIRE(state.values[3] == 4);
	REQUIRE(state.values[4] == 5);
	REQUIRE(state.values[5] == 6);
}

TEST_CASE("Get Nodes By Type", "[scenegraph]") {
	std::unique_ptr<SceneGraph> sceneGraph = std::make_unique<SceneGraph>(nullptr, nullptr);
	TestState state{};

	sceneGraph->AddChildrenToRoot(new TestSceneNode(1, nullptr),
								  new TestSceneNode(2, nullptr),
								  new TestSceneNode2(3, nullptr),
								  new TestSceneNode2(4, nullptr),
								  new TestSceneNode2(5, nullptr));

	auto nodes = sceneGraph->GetNodesByType(SceneNodeType::UNKNOWN);

	REQUIRE(nodes.size() == 2);

	// Check that our nodes contain exactly what we expect
	REQUIRE(std::any_of(nodes.begin(), nodes.end(), [](SceneNode *node) {
		return node->GetSceneNodeType() == Nimble::SceneNodeType::UNKNOWN && dynamic_cast<TestSceneNode *>(node)->GetValue() == 1;
	}));

	REQUIRE(std::any_of(nodes.begin(), nodes.end(), [](SceneNode *node) {
		return node->GetSceneNodeType() == Nimble::SceneNodeType::UNKNOWN && dynamic_cast<TestSceneNode *>(node)->GetValue() == 2;
	}));

	nodes = sceneGraph->GetNodesByType(SceneNodeType::DIRECTIONAL_LIGHT);

	REQUIRE(nodes.size() == 3);

	REQUIRE(std::any_of(nodes.begin(), nodes.end(), [](SceneNode *node) {
		return node->GetSceneNodeType() == Nimble::SceneNodeType::DIRECTIONAL_LIGHT &&
			   dynamic_cast<TestSceneNode2 *>(node)->GetValue() == 3;
	}));

	REQUIRE(std::any_of(nodes.begin(), nodes.end(), [](SceneNode *node) {
		return node->GetSceneNodeType() == Nimble::SceneNodeType::DIRECTIONAL_LIGHT &&
			   dynamic_cast<TestSceneNode2 *>(node)->GetValue() == 4;
	}));

	REQUIRE(std::any_of(nodes.begin(), nodes.end(), [](SceneNode *node) {
		return node->GetSceneNodeType() == Nimble::SceneNodeType::DIRECTIONAL_LIGHT &&
			   dynamic_cast<TestSceneNode2 *>(node)->GetValue() == 5;
	}));
}

TEST_CASE("Parent Child Scaling", "[scenegraph]") {
	// Test a simple parent -> child -> grand child, graph with scaling operations
	// applied locally on each node. Ensuring that their global scale matches expected result
	SceneGraph graph = SceneGraph(nullptr, nullptr);
	auto parent = graph.AddChildToRoot(new TestSceneNode(1, nullptr)).first;
	REQUIRE(parent != nullptr);
	auto child = parent->AddChild(new TestSceneNode(2, nullptr)).first;
	REQUIRE(child != nullptr);
	auto grandchild = child->AddChild(new TestSceneNode(3, nullptr)).first;
	REQUIRE(grandchild != nullptr);

	// Scale up the parent by 10x, which means all children should have their local transforms
	// scaled by 10x
	parent->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	REQUIRE(parent->GetGlobalTransformation().GetScale() == glm::vec3(10.0f, 10.0f, 10.0f));

	// Scale the child down by half, which when combined with its parents scale should
	// result in a global scale of 5.0
	child->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	REQUIRE(child->GetGlobalTransformation().GetScale() == glm::vec3(5.0f, 5.0f, 5.0f));

	// Scale the grand child down by another half, which should result in a global scale
	// of 2.5
	grandchild->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	REQUIRE(parent->GetGlobalTransformation().GetScale() == glm::vec3(10.0f, 10.0f, 10.0f));
	REQUIRE(child->GetGlobalTransformation().GetScale() == glm::vec3(5.0f, 5.0f, 5.0f));
	REQUIRE(grandchild->GetGlobalTransformation().GetScale() == glm::vec3(2.5f, 2.5f, 2.5f));

	grandchild->SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
	REQUIRE(grandchild->GetGlobalTransformation().GetScale() == glm::vec3(1.25f, 1.25f, 1.25f));
}

TEST_CASE("Parent Child Translation", "[scenegraph]") {
	// Test a simple parent -> child -> grandchild graph with translations
	// ensuring that children include the translations of their parents
	// and that parents do not inherit their childrens translations
	SceneGraph graph = SceneGraph(nullptr, nullptr);
	auto parent = graph.AddChildToRoot(new TestSceneNode(1, nullptr)).first;
	REQUIRE(parent != nullptr);
	auto child = parent->AddChild(new TestSceneNode(2, nullptr)).first;
	REQUIRE(child != nullptr);
	auto grandchild = child->AddChild(new TestSceneNode(3, nullptr)).first;
	REQUIRE(grandchild != nullptr);

	parent->SetTranslation(glm::vec3(10.f, 10.f, 10.f));
	REQUIRE(parent->GetGlobalTransformation().GetTranslation() == glm::vec3(10.f, 10.f, 10.f));
	child->SetTranslation(glm::vec3(10.f, 10.f, 10.f));
	REQUIRE(child->GetGlobalTransformation().GetTranslation() == glm::vec3(20.f, 20.f, 20.f));
	grandchild->SetTranslation(glm::vec3(5.f, 5.f, 5.f));
	REQUIRE(grandchild->GetGlobalTransformation().GetTranslation() == glm::vec3(25.f, 25.f, 25.f));

	// Ensure parents were not incorrectly modified
	REQUIRE(parent->GetGlobalTransformation().GetTranslation() == glm::vec3(10.f, 10.f, 10.f));
	REQUIRE(child->GetGlobalTransformation().GetTranslation() == glm::vec3(20.f, 20.f, 20.f));

	parent->SetTranslation(glm::vec3(2.f, 2.f, 2.f));
	REQUIRE(parent->GetGlobalTransformation().GetTranslation() == glm::vec3(2.f, 2.f, 2.f));
	REQUIRE(child->GetGlobalTransformation().GetTranslation() == glm::vec3(12.f, 12.f, 12.f));
	REQUIRE(grandchild->GetGlobalTransformation().GetTranslation() == glm::vec3(17.f, 17.f, 17.f));
}

TEST_CASE("Parent Child Rotation", "[scenegraph]") {
	// Test a simple parent -> child -> grandchild graph with rotations
	// ensuring that children include the rotations of their parents
	// and that parents do not inherit their childrens rotations
	SceneGraph graph = SceneGraph(nullptr, nullptr);
	auto parent = graph.AddChildToRoot(new TestSceneNode(1, nullptr)).first;
	REQUIRE(parent != nullptr);
	auto child = parent->AddChild(new TestSceneNode(2, nullptr)).first;
	REQUIRE(child != nullptr);
	auto grandchild = child->AddChild(new TestSceneNode(3, nullptr)).first;
	REQUIRE(grandchild != nullptr);

	glm::quat expectedRotation = glm::angleAxis(90.0_d, glm::vec3(1.0f, 0.0f, 0.0f));

	// Rotate around x axis by 90 degrees
	parent->SetRotation(glm::angleAxis(90_d, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))));
	REQUIRE(parent->GetGlobalTransformation().GetRotation() == expectedRotation);

	// Rotate the child around the x-y axis by 90 degrees
	child->SetRotation(glm::angleAxis(90.0_d, glm::normalize(glm::vec3(1.f, 1.f, 0.f))));
	expectedRotation = glm::angleAxis(90.0_d, glm::normalize(glm::vec3(1.f, 1.f, 0.f))) * expectedRotation;
	REQUIRE(child->GetGlobalTransformation().GetRotation() == expectedRotation);

	// Rotate the grand child around the y-z axis by 90 degrees
	grandchild->SetRotation(glm::angleAxis(90.0_d, glm::normalize(glm::vec3(0.f, 1.f, 1.f))));
	expectedRotation = glm::angleAxis(90.0_d, glm::normalize(glm::vec3(0.f, 1.f, 1.f))) * expectedRotation;
	REQUIRE(grandchild->GetGlobalTransformation().GetRotation() == expectedRotation);
}

TEST_CASE("Transformation single args", "[scenegraph]") {
	// Just ensures the short-hand functions for translate/scale/rotate
	// behave the same as the long version (which accept vectors)
	SceneGraph graph = SceneGraph(nullptr, nullptr);
	auto node1 = graph.AddChildToRoot(new TestSceneNode(1, nullptr)).first;
	auto node2 = graph.AddChildToRoot(new TestSceneNode(1, nullptr)).first;
	node1->Scale(10.f);
	node2->Scale(glm::vec3(10.f, 10.f, 10.f));

	REQUIRE(node1->GetGlobalTransformation().GetScale() == node2->GetGlobalTransformation().GetScale());

	node1->Translate(10.f);
	node2->Translate(glm::vec3(10.f, 10.f, 10.f));

	REQUIRE(node1->GetGlobalTransformation().GetTranslation() == node2->GetGlobalTransformation().GetTranslation());
}