#include <GL/glew.h>

#include "glm/gtc/matrix_transform.hpp"
#include "nimble/MeshTools.h"
#include "nimble/camera/Camera.h"
#include "nimble/engine/Engine.h"
#include "nimble/input/InputManager.h"
#include "nimble/resource-manager/ResourceManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

using namespace Nimble;

Engine::Engine(Window *window) : _window(window) {
	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);

	_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 1000.f);

	_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), .05f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	auto mesh = ResourceManager::Get().GetMesh("suzanne.blend");

	_vb = std::make_unique<VertexBuffer>(mesh.get(), BufferUsageType::Static);
	_ib = std::make_unique<IndexBuffer>(mesh.get(), BufferUsageType::Static);

	PositionNormal::SetVertexAttribPointers();
}

void Engine::RenderFrame(const Time &time) {

	static float t = 0.0f;
	t += time.dt();
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_camera->Update(time);

	ResourceManager::Get().GetMaterial("phong")->Bind();
	auto shader = ResourceManager::Get().GetMaterial("phong")->GetShader();

	glm::vec3 lightPos = glm::vec3(0.0f, -5.0f, -10.0f);
	glm::vec3 lightColor = glm::vec3(.3f, .3f, .3f);
	glm::mat4 model(1.0f);
	// auto rotationX = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// auto rotationz = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	auto rotation1 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	auto rotation2 = glm::angleAxis(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	auto position1 = glm::vec3(-2.0f, 0.0f, 0.0f);
	auto position2 = glm::vec3(2.0f, 0.0f, 0.0f);

	auto position3 = glm::vec3(0.0f, 2.0f, 0.0f);
	auto position4 = glm::vec3(0.0f, -2.0f, 0.0f);

	/*
		auto rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * rotation;

		rotation = glm::angleAxis(glm::radians((float)sin(t / 1.0f) * 325.f), glm::vec3(0.0f, 1.0f, 0.0f)) * rotation;
	*/

	auto currentRotation = glm::slerp(rotation1, rotation2, (float)((sin(t) + 1) / 2.f)) *
						   glm::angleAxis(glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	auto currentPosition = glm::lerp(position1, position2, (float)((sin(t) + 1) / 2.f)) +
						   glm::lerp(position3, position4, (float)((cos(t) + 1) / 2.f));

	// rotation.w += sin(t);


	glm::vec4 test = glm::vec4(currentPosition.x, currentPosition.y, currentPosition.z, 1.0f);

	model = model * glm::translate(currentPosition) * glm::mat4_cast(currentRotation);

	shader->SetUniform("Model", model);
	shader->SetUniform("View", _camera->GetView());
	shader->SetUniform("Projection", _projectionMatrix);
	shader->SetUniform("lightPos", lightPos);
	shader->SetUniform("lightColor", lightColor);

	_vb->Bind();
	_ib->Bind();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _ib->GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}