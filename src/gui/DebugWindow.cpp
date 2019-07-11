#include "nimble/gui/DebugWindow.h"
#include "imgui.h"

using namespace Nimble;

void DebugWindow::Draw() {
	{
		ImGui::Begin("Nimble Debug");
		{
			// Draw the FPS
			ImGui::Text("FPS: %d", (int)_engine->GetFPS());
		}
		ImGui::End();
	}
}