#include <GL/glew.h>

#include "glm/gtc/matrix_transform.hpp"
#include "nimble/MeshTools.h"
#include "nimble/engine/Engine.h"
#include "nimble/resource-manager/ResourceManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace Nimble;

Engine::Engine(Window *window) : _window(window) {
	// replace this, just for quick testing
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	this->_shader = ResourceManager::Get().GetShader("color");
	auto mesh = MeshTools::CreateColoredTriangle();

	_ib = std::make_unique<IndexBuffer>(mesh.NumIndices(), BufferUsageType::Static);
	_ib->SetData(mesh.IndexData());
	_vb = std::make_unique<VertexBuffer<PositionColor>>(mesh.VertexData(), BufferUsageType::Static);

	// NOTES: Stride requires you to count yourself + all other interleaved data
	// 3 floats of position per vertex, along with 4 floats of color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, PositionColor::SizeInBytes(), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, PositionColor::SizeInBytes(), (void *)12);
	glEnableVertexAttribArray(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Enable Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void Engine::RenderFrame(const Time &time) {
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("NimbleEngine Debug");

	ImGui::Text("FPS: %f", _fps);

	ImGui::End();

	ImGui::Render();

	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	this->_shader->Use();
	_vb->Bind();
	_ib->Bind();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	spdlog::info("{}", _fps);
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}