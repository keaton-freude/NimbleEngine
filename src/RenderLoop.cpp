#include <GL/glew.h>
#include <chrono>
#include <memory>

#include "nimble/engine/Engine.h"
#include "nimble/engine/RenderLoop.h"
#include "nimble/engine/Time.h"
#include "nimble/input/InputManager.h"
#include "nimble/opengl-wrapper/GLContext.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include "fmt/format.h"

using namespace Nimble;

RenderLoop::RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition)
: _exitCondition(exitCondition), _engine(engine) {
}

void RenderLoop::Run() {
	while(!_exitCondition()) {
		_time.Begin();

		spdlog::info("DT: {}", _time.dt());

		// Start the ImGui Frame, from here on, any component in our game loop
		// can add to the debug window
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Nimble Debug");

		// Draw the latest FPS measurement
		ImGui::Text(fmt::format("FPS: {}", _time.GetFPS()).c_str());

		// Poll for Input
		PollForEvents();

		// Render Frame
		RenderFrame(_time);

		Input::Get().ClearState();

		ImGui::End();


		// Collect all ImGui generated renderables and draw
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SwapBuffers();

		_time.End();

		_engine->SetLatestFPS(_time.GetFPS());
	}
}

void RenderLoop::RenderFrame(const Time &time) {
	_engine->RenderFrame(time);
}