#include <GL/glew.h>
#include <nimble/camera/FreeFlyCamera.h>
#include <nimble/render-passes/PhongPass.h>
#include <nimble/render-passes/ShadowPass.h>
#include <nimble/scene-graph/NullNode.h>

#include "glm/gtc/matrix_transform.hpp"
#include "nimble/MeshTools.h"
#include "nimble/camera/Camera.h"
#include "nimble/camera/FixedPointOrbitCamera.h"
#include "nimble/engine/Engine.h"
#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/scene-graph/DrawableNode.h"

using namespace Nimble;

Engine::Engine(Window *window) : _window(window) {
	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);

	// Create our perspective matrix
	auto proj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.f);
	_projectionMatrix = std::make_shared<glm::mat4>(proj);
	_camera = std::make_shared<FreeFlyCamera>(150.0f);

	_sceneGraph = std::make_unique<SceneGraph>(_projectionMatrix, _camera);

	// Add a directional light to the scene
	_rootTransformNode =
		_sceneGraph
			->AddChildToRoot(new DirectionalLightNode(DirectionalLight(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(.3f, .3f, .3f))))
			.second;

	const auto DISTANCE_BETWEEN = 5.0f;

	auto cubeNodeId = _sceneGraph->AddChild(new NullNode(), _rootTransformNode);
	cubeHandleNode = _sceneGraph->Find(cubeNodeId).value();

	for(int i = -5; i < 6; ++i) {
		for(int j = -5; j < 6; ++j) {
			auto id = _sceneGraph->AddChild(new DrawableNode("cube.fbx", "cube"), cubeNodeId);

			// Translate the node..
			glm::vec3 translation{};
			translation.x = i * DISTANCE_BETWEEN;
			translation.y = 8.0f;
			translation.z = j * DISTANCE_BETWEEN;
			_sceneGraph->Find(id).value()->Translate(translation);
		}
	}

	auto plane = MeshTools::CreateTexturedPlane(1024.0f);
	auto gridNodeId = _sceneGraph->AddChild(new DrawableNode(&plane, "grid"), _rootTransformNode);
	auto gridNode = _sceneGraph->Find(gridNodeId).value();

	auto floorId = _sceneGraph->AddChild(new DrawableNode("cube.fbx", "floor"), _rootTransformNode);
	auto floorNode = _sceneGraph->Find(floorId).value();

	floorNode->Scale(glm::vec3(50.0f, 0.01f, 50.0f));
	floorNode->Translate(glm::vec3(0.0f, 0.25f, 0.0f));

	gridNode->Scale(glm::vec3(1000.0f, 1.0f, 1000.0f));

	_shadow_pass = std::make_unique<ShadowPass>(1024 * 2, 1024 * 2);
	_phong_pass = std::make_unique<PhongPass>();

#ifndef NDEBUG
	_debug_pass = std::make_unique<DebugPass>();
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::RenderFrame(const Time &time) {

	// glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_camera->Update(time);
	auto rootTransform = _sceneGraph->Find(_rootTransformNode);
	static float total_time = 0.0f;
	total_time += time.dt();
	// cubeNode->Translate(glm::vec3(sin(total_time) * time.dt(), 0.0f, sin(total_time) * time.dt()));

	float speed = 10.0f;
	glm::vec3 translation{};
	translation.x = sin(total_time) * time.dt();
	translation.z = cos(total_time) * time.dt();
	translation *= speed;
	// cubeHandleNode->Translate(translation);

	_sceneGraph->Render();


	// glCullFace(GL_FRONT);
	_shadow_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);
	// glCullFace(GL_BACK);

	glViewport(0, 0, _window->GetWidth().get(), _window->GetHeight().get());
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifndef NDEBUG
	_debug_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);
#endif

	_phong_pass->SetShadowMap(_shadow_pass->GetShadowMap());
	_phong_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);

	static bool vsyncEnabled = _window->IsVSyncEnabled();
	if(ImGui::Checkbox("VSync Enabled", &vsyncEnabled)) {
		_window->SetVSync(vsyncEnabled);
	}

	static float cameraRotateSpeed = static_cast<FreeFlyCamera *>(_camera.get())->GetRotateSpeed();
	if(ImGui::SliderFloat("Camera Rotate Speed", &cameraRotateSpeed, 10.0f, 300.0f)) {
		static_cast<FreeFlyCamera *>(_camera.get())->SetRotateSpeed(cameraRotateSpeed);
	}

	// Horrible hacky way to limit FPS, does not take into account render time,
	// so its pretty useless except quick testing with limited FPS
	/*static int FPS = 60;
	ImGui::SliderInt("FPS", &FPS, 1, 1000);
	static size_t microseconds = 0;

	microseconds = static_cast<size_t>(((1.0f / (float)FPS) * 1000.0f) * 1000.0f);

	std::this_thread::sleep_for(std::chrono::microseconds(microseconds));*/
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}