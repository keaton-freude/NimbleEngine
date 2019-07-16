#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/exception.h>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include "nimble/engine/GlfwRenderLoop.h"
#include "nimble/input/InputManager.h"
#include "nimble/material/Material.h"
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

		Window w(Width(2560), Height(1440), "Test Title");
		GLenum err = glewInit();
		if(err != GLEW_OK) {
			spdlog::critical("Failed to initialize glew.");
			std::exit(-1);
		}

		const auto windowPointer = w.GetWindow();
		auto engine = std::make_shared<Engine>(&w);
		Input::Get().SetGlfwWindow(windowPointer);
		// Register some basic keybindings
		Input::Get().RegisterKeyCodeMapping("exit", GLFW_KEY_ESCAPE);
		// Enable Dear ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(windowPointer, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		// Register Materials manually for now
		new Material("test", "color");
		new Material("basic", "basic");

		GlfwRenderLoop renderLoop(engine, windowPointer);
		renderLoop.Run();

	} catch(std::exception &ex) {
		spdlog::critical("Unhandled exception with message: {}", ex.what());
	}

	return 0;
}
