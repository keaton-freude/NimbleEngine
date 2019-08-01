#pragma once

/*
	The Root Scene Node is the type of the root node
	within the scene graph. This is so if we want a very specific
	kind of root node, we can customize it here.

	Users who want compound customization for their nodes can define their own
	or just apply multiple nodes in the graph

	RootSceneNode contains a base transformation, some global translation
	rotation and scale.

	-- ??? Add more as they make sense to apply for a root node
*/

#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/Transformation.h"

namespace Nimble {

class RootSceneNode : public SceneNode {
private:
	// This is the root, global transformation
	Transformation _transformation;

public:
	RootSceneNode();

	// Special to the RootSceneNode, this will kick off the entire process
	// of traversing nodes and applying them
	void Render() {
		this->Visit(_transformation);
	}

	void Apply(Transformation &transformation) override {
		// Does nothing, because this is the root node
	}
};

} // namespace Nimble