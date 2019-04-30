#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <iostream>
#include <stdexcept>
#include <bits/exception.h>
#include <cstdlib>
#include <string>

#include "nimble/Mesh.h"
#include "nimble/MeshTools.h"
#include "nimble/opengl-wrapper/Buffer.h"
#include "nimble/utility/StrongTypes.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"
#include "nimble/utility/FileUtility.h"
#include "nimble/window/Window.h"
#include "nimble/resource-manager/ResourceManager.h"

#include "nimble/RenderLoop.h"
#include "nimble/GlfwRenderLoop.h"

using std::cout;
using std::endl;

using namespace Nimble;

int main() {
	try {
		// main application code here
		if (!glfwInit()) {
			throw std::runtime_error("Could not initialize GLFW");
		}

		Window w(Width(1920), Height(1080), "Test Title");
		GLenum err = glewInit();
		if(err != GLEW_OK) {
			std::cout << "Failed to initialize glew." << std::endl;
			std::exit(-1);
		}

		auto engine = std::make_shared<Engine>();

		const auto windowPointer = w.GetWindow();

		GlfwRenderLoop renderLoop(engine, windowPointer);
		renderLoop.Run();

	} catch(std::exception &ex) {
		std::cout << "Unhandled exception with message: " << ex.what() << std::endl;
	}

	return 0;
}
