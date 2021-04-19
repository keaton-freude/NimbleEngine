#include "nimble/scene-graph/DirectionalLightNode.h"

using namespace Nimble;

DirectionalLightNode::DirectionalLightNode(DirectionalLight light) : _light(light) {
	_light.enabled = true;
}

void DirectionalLightNode::Apply(SceneState &sceneState) {
}
