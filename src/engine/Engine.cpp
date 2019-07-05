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

	this->_shader = ResourceManager::Get().GetShader("color");
	auto mesh = MeshTools::CreateColoredTriangle();

	_ib = std::make_unique<IndexBuffer>(mesh.NumIndices(), BufferUsageType::Static);
	_ib->SetData(mesh.IndexData());
	_vb = std::make_unique<VertexBuffer<PositionColor>>(mesh.VertexData(), BufferUsageType::Static);

	// NOTES: Stride requires you to count yourself + all other interleaved data
	// 3 floats of position per vertex, along with 4 floats of color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)12);
	glEnableVertexAttribArray(1);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::RenderFrame() {
	spdlog::info("Ping");
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->_shader->Use();
	_vb->Bind();
	_ib->Bind();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	for(int j = 0; j < 1; ++j) {
		for(int i = 0; i < 2; ++i) {
			_textRenderer->RenderText("TestText", 100.0f * j, 20.0f * i, 1.0f, glm::vec3({ .7f, .7f, .7f }));
		}
	}
}