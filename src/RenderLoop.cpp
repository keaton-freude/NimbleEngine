#include <array>
#include <memory>
#include <utility>

#include "nimble/engine/Engine.h"
#include "nimble/engine/RenderLoop.h"
#include "nimble/input/InputManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"


using namespace Nimble;

RenderLoop::RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition)
: _exitCondition(std::move(exitCondition)), _engine(std::move(engine)) {
}

void RenderLoop::Run() {
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

		// Render Frame
		RenderFrame(_time);

		PlotFrameTime();

		Input::Get().ClearState();

		ImGui::End();


		// Collect all ImGui generated renderables and draw
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SwapBuffers();

		_time.End();


		spdlog::info("Frametime: {}", _time.GetFrameTime());

		_engine->SetLatestFPS(_time.GetFPS());
	}
}

void RenderLoop::RenderFrame(const Time &time) {
	_engine->RenderFrame(time);
}

void RenderLoop::PlotFrameTime() {
	const auto frameTimeIter = _time.GetFrameTimeHistoryIterator();
	const auto numFrames = _time.GetNumHistoryFrames();

	if (numFrames) {
		ImPlot::SetNextPlotLimits(0.f, 50.f, 0.0, _time.GetMaxFrameTimeHistory(), ImGuiCond_Always);
		if(ImPlot::BeginPlot("Frametime", nullptr, "Frametime (ms)")) {
			ImPlot::PlotLine("Test Plot", frameTimeIter, numFrames);
			ImPlot::EndPlot();
		}
	}
}