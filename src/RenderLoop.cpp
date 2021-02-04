#include <array>
#include <memory>
#include <nimble/subsystem/FileWatcherSubsystem.h>
#include <thread>
#include <utility>

#include "nimble/engine/Engine.h"
#include "nimble/engine/RenderLoop.h"
#include "nimble/input/InputManager.h"
#include "nimble/resource-manager/ResourceManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"


using namespace Nimble;

RenderLoop::RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition)
: _exitCondition(std::move(exitCondition)), _engine(std::move(engine)) {
	auto& system = _subsystems.emplace_back(std::unique_ptr<ISubsystem>(
	new FileWatcherSubsystem(ResourceManager::Get().GetResourceDirectoryByName("shaders"), ChangeType::FILE_CHANGED)));

	ResourceManager::Get().RegisterFileChange(static_cast<FileWatcherSubsystem*>(system.get())->FileModifiedEvent);

}

void RenderLoop::Run() {
	for (const auto& subsystem : _subsystems) {
		subsystem->OnCreate();
	}
	while(!_exitCondition()) {
		_time.Begin();

		// Start the ImGui Frame, from here on, any component in our game loop
		// can add to the debug window
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Nimble Debug");

		// Draw the latest FPS measurement
		ImGui::Text("%s", fmt::format("FPS: {}", _time.GetFPS()).c_str());

		// Poll for Input
		PollForEvents();

		// Process every installed sub system
		for(const auto &subsystem : _subsystems) {
			subsystem->OnTick(_time.dt());
		}

		if(Input::Get().IsKeyPressed("reload_shaders")) {
			ResourceManager::Get().ReloadShaders();
		}

		// Render Frame
		RenderFrame(_time);

		Input::Get().ClearState();
		// ImPlot::ShowDemoWindow();

		ImGui::End();


		// Collect all ImGui generated renderables and draw
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SwapBuffers();

		_time.End();


		_engine->SetLatestFPS(_time.GetFPS());

#ifdef _WIN32
		// On Windows, we need the main thread to enter an alertable
		// wait state, which allows the kernel to process any callbacks
		// for asynchronous operations
		// We'll sleep for the minimum amount of time
		SleepEx(0, true);
#endif
	}

	for (const auto& subsystem : _subsystems) {
		subsystem->OnDestroy();
	}
}

void RenderLoop::RenderFrame(const Time &time) {
	_engine->RenderFrame(time);
}
