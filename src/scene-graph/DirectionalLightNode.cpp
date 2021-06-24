#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/utility/ImGuiUtility.h"

using namespace Nimble;

DirectionalLightNode::DirectionalLightNode(DirectionalLight light, const std::string &name)
: SceneNode(name), _light(light) {
	_light.direction = glm::normalize(light.direction);
	_light.enabled = true;
}

void DirectionalLightNode::Apply(SceneState &sceneState) {
}

void DirectionalLightNode::DrawGuiElements() {
	GUI_SLIDER_FLOAT3(direction, _light.direction, -1.0f, 1.0f);
	_light.direction = glm::normalize(direction);

	GUI_SLIDER_FLOAT3(position, _light.position, -100.0f, 100.0f);
	_light.position = position;

	GUI_SLIDER_FLOAT1(projWidth, _light.projection.width, -400.0f, 400.0f);
	_light.projection.width = projWidth;

	GUI_SLIDER_FLOAT1(projHeight, _light.projection.height, -400.0f, 400.0f);
	_light.projection.height = projHeight;

	GUI_SLIDER_FLOAT1(projDepth, _light.projection.depth, 10.f, 1000.0f);
	_light.projection.depth = projDepth;
}
