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

#include "nimble/scene-graph/SceneNode.h"

namespace Nimble {

class SceneGraph {
private:
	// Probably convert this to some specific RootSceneNode
	// Maybe something which supports a variety of what will end up being
	// global transformations for the entire scene
	std::unique_ptr<SceneNode> _rootNode;

public:
	SceneGraph();
};

}; // namespace Nimble