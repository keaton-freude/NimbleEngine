#include <GL/glew.h>

#include "nimble/MeshTools.h"
#include "nimble/engine/Engine.h"
#include "nimble/resource-manager/ResourceManager.h"

using namespace Nimble;

Engine::Engine() {
	_textRenderer = std::make_unique<TextRenderer>();
	// Hack to get dimensions.. we should probably be creating the text renderer
	// outside of the engine, then injecting it into the engine
	// handling resizes seems tricky. Always listening to the glContext seems a
	// reliable way to get the info, but how can we know when its been changed?
	GLint dims[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, dims);
	_textRenderer->SetProjectionDimensions(dims[2], dims[3]);

	// replace this, just for quick testing
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	this->_shader = ResourceManager::Get().GetShader("basic");

	_shader->Use();

	auto mesh = MeshTools::CreateTriangle();

	_ib = std::make_unique<IndexBuffer>(mesh.NumIndices(), BufferUsageType::Static);
	_ib->SetData(mesh.IndexData());
	_vb = std::make_unique<VertexBuffer<Position>>(mesh.VertexData(), BufferUsageType::Static);

	glBindVertexArray(_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::RenderFrame() {
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->_shader->Use();
	_vb->Bind();
	_ib->Bind();

	glBindVertexArray(_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	_textRenderer->RenderText("TestText", 200.0f, 200.0f, 1.0f, glm::vec3({ .7f, .7f, .7f }));
}