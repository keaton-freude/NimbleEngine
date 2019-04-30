#include <GL/glew.h>

#include "nimble/engine/Engine.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/MeshTools.h"

using namespace Nimble;

Engine::Engine() {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Engine::RenderFrame() {
    glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->_shader->Use();
    _vb->Bind();
    _ib->Bind();

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}