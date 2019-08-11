#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "nimble/camera/Camera.h"
#include "nimble/engine/Time.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"
#include "nimble/scene-graph/SceneGraph.h"

#include "nimble/window/Window.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>

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

	float GetFPS() const {
		return _fps;
	}

private:
	// std::unique_ptr<VertexBuffer> _vb;
	// std::unique_ptr<IndexBuffer> _ib;
	// unsigned int _vao;
	float _fps;
	Window *_window;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<glm::mat4> _projectionMatrix;
	std::unique_ptr<SceneGraph> _sceneGraph;
};

} // namespace Nimble