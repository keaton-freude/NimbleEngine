#pragma once

/*
	The Root Scene Node is the type of the root node
	within the scene graph. This is so if we want a very specific
	kind of root node, we can customize it here.

	Users who want compound customization for their nodes can define their own
	or just apply multiple nodes in the graph

	RootSceneNode supports, thus far:
	- Translation
	- Rotation
	- Scaling

	-- ??? Add more as they make sense to apply for a root node
*/

#include "nimble/scene-graph/SceneNode.h"

namespace Nimble {

class RootSceneNode : public SceneNode {
private:
public:
	RootSceneNode();

	void Apply() override {
	}
};

} // namespace Nimble