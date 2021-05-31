#pragma once

#define GUI_SLIDER_FLOAT1(type, variableName, initialValue, min, max, setter) \
	static type variableName = initialValue; \
	if (ImGui::SliderFloat(#variableName, &variableName, min, max)) {   \
    	setter(variableName);                                           \
	}