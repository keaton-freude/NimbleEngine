#include <GL/glew.h>

#include "glm/gtc/matrix_transform.hpp"
#include "nimble/MeshTools.h"
#include "nimble/camera/Camera.h"
#include "nimble/engine/Engine.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/scene-graph/DirectionalLightNode.h"
#include "nimble/scene-graph/DrawableNode.h"
#include "nimble/scene-graph/TransformNode.h"

#include "imgui.h"


using namespace Nimble;

Engine::Engine(Window *window) : _window(window) {
	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);

	// Create our perspective matrix
	auto proj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.f);
	// Create a pointer via copy constructor
	_projectionMatrix = std::make_shared<glm::mat4>(proj);
	_camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), .05f);

	_sceneGraph = std::make_unique<SceneGraph>(_projectionMatrix, _camera);

	// Add a directional light to the scene
	_rootTransformNode = _sceneGraph
						 ->AddChildToRoot(new DirectionalLightNode(
						 DirectionalLight(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(.3f, .3f, .3f))))
						 .first->AddChild(new TransformNode(Transformation()))
						 .second;

	// Add some suzannes
	for(size_t j = 0; j < 10; ++j) {
		for(size_t i = 0; i < 10; ++i) {
			Transformation transform;
			transform.Translate(glm::vec3((i * 3.0f) + (-10.0f), (j * 2.5f) + (-10.0f), 0.0f));
			transform.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(90.0f));
			transform.Rotate(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(180.0f));
			_sceneGraph->AddChild(new DrawableNode<PositionNormal>("suzanne.blend", "phong", transform),
								  _rootTransformNode);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/*glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	auto mesh = ResourceManager::Get().GetMesh("suzanne.blend");

	_vb = std::make_unique<VertexBuffer>(mesh.get(), BufferUsageType::Static);
	_ib = std::make_unique<IndexBuffer>(mesh.get(), BufferUsageType::Static);

	PositionNormal::SetVertexAttribPointers();*/
}

void Engine::RenderFrame(const Time &time) {

	static float t = 0.0f;
	t += time.dt();
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
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
		rotZ.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(0.1f));
		transform = transform * rotZ;
	}

	_sceneGraph->Render();
	/*
		ResourceManager::Get().GetMaterial("phong")->Bind();
		auto shader = ResourceManager::Get().GetMaterial("phong")->GetShader();

		glm::vec3 lightPos = glm::vec3(0.0f, -5.0f, -10.0f);
		glm::vec3 lightColor = glm::vec3(.3f, .3f, .3f);

		auto rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * rotation;

		glm::vec4 test = glm::vec4(currentPosition.x, currentPosition.y, currentPosition.z, 1.0f);

		// model = model * glm::translate(currentPosition) * glm::mat4_cast(currentRotation);

		// shader->SetUniform("Model", model);
		shader->SetUniform("View", _camera->GetView());
		shader->SetUniform("Projection", _projectionMatrix);
		shader->SetUniform("lightPos", lightPos);
		shader->SetUniform("lightColor", lightColor);

		_vb->Bind();
		_ib->Bind();

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _ib->GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
	*/
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}