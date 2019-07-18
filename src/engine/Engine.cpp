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

using namespace Nimble;

Engine::Engine(Window *window) : _window(window) {
	// replace this, just for quick testing
	/*auto mesh = MeshTools::CreateColoredTriangle();

	_ib = std::make_unique<IndexBuffer>(mesh.NumIndices(), BufferUsageType::Static);
	_ib->SetData(mesh.IndexData());
	_vb = std::make_unique<VertexBuffer<PositionColor>>(mesh.VertexData(), BufferUsageType::Static);

	PositionColor::SetVertexAttribPointers();*/

	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);

	_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 1000.f);

	_camera = new Camera();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	auto mesh = ResourceManager::Get().GetMesh("test.blend");
	// auto mesh = MeshTools::CreateTriangle();

	_vb = std::make_unique<VertexBuffer>(mesh.get(), BufferUsageType::Static);
	_ib = std::make_unique<IndexBuffer>(mesh.get(), BufferUsageType::Static);

	Position::SetVertexAttribPointers();
}

void Engine::RenderFrame(const Time &time) {
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto mouse = Input::Get().GetMouseMovement();
	_camera->Rotate(mouse * time.dt());

	//_camera->Update(time);

	ResourceManager::Get().GetMaterial("basic")->Bind();
	glm::mat4 MVP = _projectionMatrix * _camera->GetView() * glm::mat4(1.0f);
	GLuint MatrixID =
	glGetUniformLocation(ResourceManager::Get().GetMaterial("basic")->GetShader()->ShaderHandle(), "MVP");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	_vb->Bind();
	_ib->Bind();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _ib->GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}