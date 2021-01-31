#include <GL/glew.h>

#ifdef _MSC_VER
#pragma warning( disable : 4005 )
#endif
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include "nimble/engine/GlfwRenderLoop.h"
#include "nimble/input/InputManager.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "cxxopts.hpp"

using namespace Nimble;

cxxopts::ParseResult HandleCLIArgs(int argc, char** argv);

int main(int argc, char** argv) {
	try {
		auto args = HandleCLIArgs(argc, argv);
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

		const auto windowPointer = w.GetWindow();
		ResourceManager::Get().SetResourceRoot(args["resource-root"].as<std::string>());
		ResourceManager::Get().LoadMaterialsFromDisk();
		// Register Materials manually for now

		auto engine = std::make_shared<Engine>(&w);
		Input::Get().SetGlfwWindow(windowPointer);
		// Register some basic keybindings
		Input::Get().RegisterKeyCodeMapping("exit", GLFW_KEY_ESCAPE);
		Input::Get().RegisterKeyCodeMapping("reload_shaders", GLFW_KEY_F5);
		// Enable Dear ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO &io = ImGui::GetIO();
		(void)io;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(windowPointer, true);
		ImGui_ImplOpenGL3_Init("#version 460");


		GlfwRenderLoop renderLoop(engine, windowPointer);
		renderLoop.Run();

	} catch(std::exception &ex) {
		spdlog::critical("Unhandled exception with message: {}", ex.what());
	}

	return 0;
}

cxxopts::ParseResult HandleCLIArgs(int argc, char** argv) {
	cxxopts::Options options("NimbleEngineFrontend", "Example frontend application of NimbleEngine");

	options.add_options()
		("r,resource-root", "Resource Root Directory", cxxopts::value<std::string>()->default_value("./resources")->no_implicit_value())
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);

	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		std::exit(0);
	}

	return result;
}