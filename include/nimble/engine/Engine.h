#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "nimble/engine/Time.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"

#include "nimble/window/Window.h"

// Contains the top-level object for the Nimble Rendering Engine
// Contains all of the methods for interacting with the Engine
// and utilizing it.

namespace Nimble {

class Engine {
public:
	Engine(Window *window);
	void RenderFrame(const Time &time);

	// Tell the engine what its latest FPS is, so it can render it
	void SetLatestFPS(float FPS);

private:
	std::unique_ptr<VertexBuffer<PositionColor>> _vb;
	std::unique_ptr<IndexBuffer> _ib;
	std::shared_ptr<ShaderProgram> _shader;
	unsigned int _vao;

	float _fps;
	Window *_window;
};

} // namespace Nimble