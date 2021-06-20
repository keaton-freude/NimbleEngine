#pragma once

#include "nimble/scene-graph/SceneNode.h"

/**
 * 	Null Node is just a regular SceneNode with nothing else
 * 	This can be useful as a control point for controlling a set
 * 	of nodes without needing a real node
 */

namespace Nimble {
class NullNode : public SceneNode {
private:
public:
	NullNode() : SceneNode("null_node") {
	}

	void Apply(SceneState &sceneState) override {
	}

	static SceneNodeType SCENE_NODE_TYPE() {
		return SceneNodeType::NULL_NODE;
	}

	SceneNodeType GetSceneNodeType() override {
		return SCENE_NODE_TYPE();
	}
};
} // namespace Nimble