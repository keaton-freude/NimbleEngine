#include "nimble/scene-graph/DirectionalLightNode.h"

using namespace Nimble;

DirectionalLightNode::DirectionalLightNode(DirectionalLight light) : _light(light) {
	_light.enabled = true;
}

void DirectionalLightNode::Apply(SceneState &sceneState) {
	static glm::vec3 direction {};
	ImGui::SliderFloat3("Light Direction", &_light.direction.x, -1.0f, 1.0f);
	_light.direction = glm::normalize(_light.direction);
}