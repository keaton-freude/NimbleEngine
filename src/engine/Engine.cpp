#include <GL/glew.h>
#include <nimble/camera/FreeFlyCamera.h>
#include <nimble/render-passes/PhongPass.h>
#include <nimble/scene-graph/NullNode.h>
#include <nimble/utility/ImGuiUtility.h>

#include "nimble/MeshTools.h"
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

	glm::vec3 lightDirection = glm::vec3(1.0f, -1.0f, -1.0f);

	// Add a directional light to the scene
	_rootTransformNode = _sceneGraph
							 ->AddChildToRoot(new DirectionalLightNode(
								 DirectionalLight(lightDirection,
												  glm::vec3(.3f, .3f, .3f),
												  glm::vec3(0.0f, 0.0f, 0.0f),
												  OrthoProjection(100.0f, -100.0f, 100.0f, -100.0f, -0.1f, 100.0f))))
							 .second;

	const auto DISTANCE_BETWEEN = 5.0f;

	auto cubeNodeId = _sceneGraph->AddChild(new NullNode(), _rootTransformNode);

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

	_shadow_pass = std::make_unique<ShadowPass>(1024 * 16, 1024 * 16);
	_phong_pass = std::make_unique<PhongPass>();

#ifndef NDEBUG
	_debug_pass = std::make_unique<DebugPass>();
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::RenderFrame(const Time &time) {
	_camera->Update(time);
	auto rootTransform = _sceneGraph->Find(_rootTransformNode);
	static float total_time = 0.0f;
	total_time += time.dt();

	float speed = 10.0f;
	glm::vec3 translation{};
	translation.x = sin(total_time) * time.dt();
	translation.z = cos(total_time) * time.dt();
	translation *= speed;

	_sceneGraph->Render();


	_shadow_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);

	glViewport(0, 0, (int)_window->GetWidth().get(), (int)_window->GetHeight().get());
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

	auto flyCamera = std::dynamic_pointer_cast<FreeFlyCamera>(_camera);
	GUI_SLIDER_FLOAT1_WITH_SETTER_FUNC(cameraRotateSpeed, flyCamera->GetRotateSpeed(), 10.0f, 300.0f, flyCamera->SetRotateSpeed);
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}