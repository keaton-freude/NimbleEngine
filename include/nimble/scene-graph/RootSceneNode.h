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

#include "nimble/camera/Camera.h"
#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/SceneState.h"
#include <glm/glm.hpp>
#include <memory>

namespace Nimble {

class RootSceneNode : public SceneNode {
private:
	SceneState _sceneState;

public:
	RootSceneNode() = delete;

	RootSceneNode(std::shared_ptr<glm::mat4> projectionMatrix, std::shared_ptr<ICamera> camera);

	// Special to the RootSceneNode, this will kick off the entire process
	// of traversing nodes and applying them
	void Render() {
		this->Visit(_sceneState);
	}

	void Apply(SceneState &sceneState) override {
		// Does nothing, because this is the root node
	}

	SceneNodeType GetSceneNodeType() override {
		return SceneNodeType::ROOT;
	}
};

} // namespace Nimble