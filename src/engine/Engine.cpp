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
							 ->AddChildToRoot(new DirectionalLightNode(DirectionalLight(lightDirection,
																						glm::vec3(.3f, .3f, .3f),
																						glm::vec3(-62.5f, 22.5f, 15.0f),
																						OrthoProjection(225.f, 150.f, 120.f)),
																	   "directional_light"))
							 .second;

	const auto DISTANCE_BETWEEN = 5.0f;

	auto cubeNodeId = _sceneGraph->AddChild(new NullNode(), _rootTransformNode);

	auto id2 = _sceneGraph->AddChild(new DrawableNode("cube.fbx", "cube", "cube"), cubeNodeId);
	_rockNode = _sceneGraph->Find(id2).value();
	_rockNode->Scale(glm::vec3(3.f, 3.f, 3.f));
	_rockNode->Translate(glm::vec3(0.0f, 15.0f, 0.0f));

	auto id3 = _sceneGraph->AddChild(new DrawableNode("cube.fbx", "cube", "cube"), id2);
	_rockNode2 = _sceneGraph->Find(id3).value();
	_rockNode2->Scale(glm::vec3(3.f, 3.f, 3.f));
	_rockNode2->Translate(glm::vec3(0.0f, 25.0f, 0.0f));

	auto plane = MeshTools::CreateTexturedPlane(1024.0f);
	auto gridNodeId = _sceneGraph->AddChild(new DrawableNode(&plane, "grid", "grid"), _rootTransformNode);
	auto gridNode = _sceneGraph->Find(gridNodeId).value();
	gridNode->Translate(glm::vec3(0.0f, -1.0f, 0.0f));

	auto floorId = _sceneGraph->AddChild(new DrawableNode("cube.fbx", "floor", "floor"), _rootTransformNode);
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

	_sceneGraph->RenderGui();
	_sceneGraph->Render();

	_shadow_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);

	glViewport(0, 0, (int)_window->GetWidth().get(), (int)_window->GetHeight().get());
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_phong_pass->SetShadowMap(_shadow_pass->GetShadowMap());
	_phong_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);

#ifndef NDEBUG
	_debug_pass->Draw(_sceneGraph->GetRootNode()->GetSceneState(), *_sceneGraph);
#endif

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