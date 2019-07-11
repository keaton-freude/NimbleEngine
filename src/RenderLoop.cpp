#include <GL/glew.h>
#include <chrono>
#include <memory>

#include "nimble/engine/Engine.h"
#include "nimble/engine/RenderLoop.h"
#include "nimble/engine/Time.h"
#include "nimble/opengl-wrapper/GLContext.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

using namespace Nimble;

RenderLoop::RenderLoop(std::shared_ptr<Engine> engine, ExitCondition exitCondition)
: _exitCondition(exitCondition), _engine(engine), _debugWindow(std::make_unique<DebugWindow>(engine)) {
}

void RenderLoop::Run() {
	while(!_exitCondition()) {
		_time.Begin();

		// Poll for Input
		PollForEvents();

		// Render Frame
		RenderFrame(_time);

		// Start the ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Draw our Debug Window
		// NOTE: Expand this to be more generic
		// Maybe keep a list of all Windows we want to draw..?
		_debugWindow->Draw();

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