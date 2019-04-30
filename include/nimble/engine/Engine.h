#pragma once

#include <memory>

#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"

// Contains the top-level object for the Nimble Rendering Engine
// Contains all of the methods for interacting with the Engine
// and utilizing it.

namespace Nimble
{

class Engine {
public:
    Engine();
    void RenderFrame();
private:
    std::unique_ptr<VertexBuffer<Position>> _vb;
    std::unique_ptr<IndexBuffer> _ib;
    std::shared_ptr<ShaderProgram> _shader;
    unsigned int _vao;
};

}