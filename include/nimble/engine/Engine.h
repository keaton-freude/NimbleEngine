#pragma once

#include <memory>

#include "nimble/engine/Time.h"
#include "nimble/font-rendering/TextRenderer.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"

// Contains the top-level object for the Nimble Rendering Engine
// Contains all of the methods for interacting with the Engine
// and utilizing it.

namespace Nimble {

class Engine {
public:
	Engine();
	void RenderFrame(const Time &time);

	// Tell the engine what its latest FPS is, so it can render it
	void SetLatestFPS(float FPS);

private:
	std::unique_ptr<VertexBuffer<PositionColor>> _vb;
	std::unique_ptr<IndexBuffer> _ib;
	std::shared_ptr<ShaderProgram> _shader;
	unsigned int _vao;
	std::unique_ptr<TextRenderer> _textRenderer;

	float _fps;
};

} // namespace Nimble