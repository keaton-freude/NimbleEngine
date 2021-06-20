#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/utility/ImGuiUtility.h"

using namespace Nimble;

DirectionalLightNode::DirectionalLightNode(DirectionalLight light, const std::string &name)
: SceneNode(name), _light(light) {
	_light.direction = glm::normalize(light.direction);
	_light.enabled = true;
}

void DirectionalLightNode::Apply(SceneState &sceneState) {
	if(ImGui::TreeNode("Directional Light")) {
		GUI_SLIDER_FLOAT3(lightDirection, _light.direction, -1.0f, 1.0f);
		_light.direction = glm::normalize(lightDirection);

		GUI_SLIDER_FLOAT3(lightPosition, _light.position, -100.0f, 100.0f);
		_light.position = lightPosition;

		GUI_SLIDER_FLOAT1(projectionWidth, _light.projection.width, -400.0f, 400.0f);
		_light.projection.width = projectionWidth;

		GUI_SLIDER_FLOAT1(projectionHeight, _light.projection.height, -400.0f, 400.0f);
		_light.projection.height = projectionHeight;

		GUI_SLIDER_FLOAT1(projectionDepth, _light.projection.depth, 10.f, 1000.0f);
		_light.projection.depth = projectionDepth;
		ImGui::TreePop();
	}
}