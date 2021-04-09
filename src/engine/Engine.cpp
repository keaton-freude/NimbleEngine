#include <GL/glew.h>
#include <nimble/camera/FreeFlyCamera.h>

#include "glm/gtc/matrix_transform.hpp"
#include "nimble/MeshTools.h"
#include "nimble/camera/Camera.h"
#include "nimble/camera/FixedPointOrbitCamera.h"
#include "nimble/engine/Engine.h"
#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/scene-graph/DrawableNode.h"
#include "nimble/scene-graph/TransformNode.h"

using namespace Nimble;

Engine::Engine(Window *window) : _window(window) {
	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);

	// Create our perspective matrix
	auto proj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.f);
	// Create a pointer via copy constructor
	_projectionMatrix = std::make_shared<glm::mat4>(proj);
	_camera = std::make_shared<FreeFlyCamera>();

	_sceneGraph = std::make_unique<SceneGraph>(_projectionMatrix, _camera);

	// Add a directional light to the scene
	_rootTransformNode = _sceneGraph
						 ->AddChildToRoot(new DirectionalLightNode(
						 DirectionalLight(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(.3f, .3f, .3f))))
						 .first->AddChild(new TransformNode(Transformation()))
						 .second;

	Transformation transform;
	Transformation cubeTransform;
	cubeTransform.Translate(glm::vec3(0.0f, 1.01f, 0.0f));
	_sceneGraph->AddChild(new DrawableNode("cube.fbx", "cube", cubeTransform), _rootTransformNode);
	transform.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 90.f * AI_MATH_PI_F/180.f);
	transform.Scale(glm::vec3(1000.0f, 1000.0f, 1000.0f));
	auto plane = MeshTools::CreateTexturedPlane();
	_sceneGraph->AddChild(new DrawableNode(&plane, "grid", transform), _rootTransformNode);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::RenderFrame(const Time &time) {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_camera->Update(time);
	auto rootTransform = _sceneGraph->Find(_rootTransformNode);

	/*
		Note for later: We need to apply local transforms, before applying
		the overall scene transform. We also need some way for the user to specify
		the ordering of transform multiply (scale -> rotate -> translate) vs
		(scale -> translate -> rotate), etc
	*/

	if(rootTransform.has_value()) {
		TransformNode *transformNode = dynamic_cast<TransformNode *>(rootTransform.value());
		auto &transform = transformNode->GetTransform();

		// Rotate a little around the Z
		Transformation rotZ;
		//rotZ.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(60.0f) * time.dt());
		transform = transform * rotZ;
	}

	_sceneGraph->Render();

	static bool vsyncEnabled = false;
	if (ImGui::Checkbox("VSync Enabled", &vsyncEnabled)) {
		_window->SetVSync(vsyncEnabled);
	}

	static int FPS = 60;
	ImGui::SliderInt("FPS", &FPS, 1, 1000);
	static size_t microseconds = 0;

	microseconds = static_cast<size_t>(((1.0f / (float)FPS) * 1000.0f) * 1000.0f);

	std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
	//spdlog::info("Delay: {}", microseconds);
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}