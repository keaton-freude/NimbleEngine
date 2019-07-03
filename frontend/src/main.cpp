#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/exception.h>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include "spdlog/spdlog.h"

#include "nimble/GlfwRenderLoop.h"
#include "nimble/font-rendering/TextRenderer.h"
#include "nimble/input/InputManager.h"
#include "nimble/utility/StrongTypes.h"
#include "nimble/window/Window.h"

using namespace Nimble;

int main() {
	try {
		spdlog::set_level(spdlog::level::debug);
		// main application code here
		if(!glfwInit()) {
			throw std::runtime_error("Could not initialize GLFW");
		}

		Window w(Width(1920), Height(1080), "Test Title");
		GLenum err = glewInit();
		if(err != GLEW_OK) {
			spdlog::critical("Failed to initialize glew.");
			std::exit(-1);
		}

		auto engine = std::make_shared<Engine>();
		const auto windowPointer = w.GetWindow();
		Input::Get().SetGlfwWindow(windowPointer);
		// Register some basic keybindings
		Input::Get().RegisterKeyCodeMapping("exit", GLFW_KEY_ESCAPE);

		GlfwRenderLoop renderLoop(engine, windowPointer);
		renderLoop.Run();

	} catch(std::exception &ex) {
		spdlog::critical("Unhandled exception with message: {}", ex.what());
	}

	return 0;
}
