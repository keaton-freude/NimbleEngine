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
	/*auto mesh = MeshTools::CreateColoredTriangle();

	_ib = std::make_unique<IndexBuffer>(mesh.NumIndices(), BufferUsageType::Static);
	_ib->SetData(mesh.IndexData());
	_vb = std::make_unique<VertexBuffer<PositionColor>>(mesh.VertexData(), BufferUsageType::Static);

	PositionColor::SetVertexAttribPointers();*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Assimp::Importer importer;
	_scene = importer.ReadFile("../resources/models/suzanne.blend",
							   aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if(!_scene) {
		spdlog::error("Failed to load suzanne. Error: {}", importer.GetErrorString());
		return;
	}
	// do something...

	// Get meshes?
	spdlog::info("Number of verts: {}", _scene->mMeshes[0]->mNumVertices);
	spdlog::info("Number of triangles: {}", _scene->mMeshes[0]->mNumFaces);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	auto mesh = ResourceManager::Get().GetMesh("suzanne.blend");


	_vb = std::make_unique<VertexBuffer>(mesh.get(), BufferUsageType::Static);
	_ib = std::make_unique<IndexBuffer>(mesh.get(), BufferUsageType::Static);

	Position::SetVertexAttribPointers();
}

void Engine::RenderFrame(const Time &time) {
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ResourceManager::Get().GetMaterial("basic")->Bind();
	_vb->Bind();
	_ib->Bind();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _ib->GetNumFaces(), GL_UNSIGNED_INT, 0);

	int width, height;
	glfwGetFramebufferSize(_window->GetWindow(), &width, &height);
}

void Engine::SetLatestFPS(float FPS) {
	_fps = FPS;
}