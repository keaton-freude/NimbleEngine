#pragma once

/*
	NOTE: All of this subject to change

	A scene graph is a way of organzing translations, rotations, animations, rendering,
	object attachment and much more in a 3D scene.

	We have a variety of different Node Types
	RenderNode - Renders some attached geometry
	MaterialNode - Contains material settings to be applied
		Materials contain bound shaders, require Uniforms to be set, textures to be bound
		Along with OpenGL render state like blend mode, culling, whatever

		NOTE: We may need to limit this such that only 1 MaterialNode can be applied
		at once, or it somehow overwrites the previous one, because mixing materials is probably
		going to be confusing and not work well (how to pick a "mixed" shader?, vertex offsets
		etc)
	TransformNode - This Node modifies the SceneGraphs Current transform value
		A transform is some state of Translation, Rotation, Scale
	RotateNode - Apply some rotation to the current world matrix
	ScaleNode - Apply some scaling operation to the current world matrix
	TranslateNode - Apply a translation to the current world matrix

*/

#include "nimble/scene-graph/RootSceneNode.h"
#include "nimble/scene-graph/SceneNode.h"

namespace Nimble {

class SceneGraph {
private:
	// Probably convert this to some specific RootSceneNode
	// Maybe something which supports a variety of what will end up being
	// global transformations for the entire scene
	std::unique_ptr<RootSceneNode> _rootNode;

public:
	// Use the default root node type (All-axis transform)
	SceneGraph();

	// Otherwise use specified one
	SceneGraph(RootSceneNode *node);

	// Apply the graph. Walk every node, applying the node
	// This can result in transformations, meshes being drawn, etc
	void Render();

	// Take ownership of a raw pointer, similar to the
	// SceneNode::AddChild
	size_t AddChildToRoot(SceneNode *node);

	size_t AddChild(SceneNode *node, size_t id);
	size_t AddChild(std::unique_ptr<SceneNode> &&node, size_t id);

private:
	// Look through all nodes in the graph for `id`
	const std::optional<SceneNode *const> Find(size_t id);
};

}; // namespace Nimble