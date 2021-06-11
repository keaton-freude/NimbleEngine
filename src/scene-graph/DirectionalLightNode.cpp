#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/utility/ImGuiUtility.h"

using namespace Nimble;

DirectionalLightNode::DirectionalLightNode(DirectionalLight light) : _light(light) {
	_light.enabled = true;
}

void DirectionalLightNode::Apply(SceneState &sceneState) {
	if(ImGui::TreeNode("Directional Light")) {
		GUI_SLIDER_FLOAT3(lightDirection, _light.direction, -1.0f, 1.0f);
		_light.direction = glm::normalize(lightDirection);

		GUI_SLIDER_FLOAT3(lightPosition, _light.position, -100.0f, 100.0f);
		_light.position = lightPosition;

		GUI_SLIDER_FLOAT1(projectionWidth, _light.projection._right, -200.0f, 200.0f);
		_light.projection._right = projectionWidth;
		_light.projection._left = -projectionWidth;

		GUI_SLIDER_FLOAT1(projectionHeight, _light.projection._top, -200.0f, 200.0f);
		_light.projection._top = projectionHeight;
		_light.projection._bottom = -projectionHeight;

		GUI_SLIDER_FLOAT1(projectionNear, _light.projection._near, 0.0f, 10.0f);
		_light.projection._near = projectionNear;

		GUI_SLIDER_FLOAT1(projectionFar, _light.projection._far, 100.0f, 1000.0f);
		_light.projection._far = projectionFar;
		ImGui::TreePop();
	}
}