#pragma once

/**
 * Encapsulates a DirectionalLight, such that all descendent
 * nodes will be affected by this light (barring any per-node settings),
 * such as those that might limit lighting.
 */

#include "nimble/light/DirectionalLight.h"
#include "nimble/scene-graph/SceneNode.h"

namespace Nimble {
class DirectionalLightNode : public SceneNode {
private:
	DirectionalLight _light;

public:
	// require user to give us a light at construction time
	DirectionalLightNode() = delete;

	DirectionalLightNode(DirectionalLight light, const std::string &name);

	void Apply(SceneState &sceneState) override;

	static SceneNodeType SCENE_NODE_TYPE() {
		return SceneNodeType::DIRECTIONAL_LIGHT;
	}

	SceneNodeType GetSceneNodeType() override {
		return SCENE_NODE_TYPE();
	}

	DirectionalLight GetDirectionalLight() {
		return _light;
	}
};
} // namespace Nimble