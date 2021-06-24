#pragma once

#define GUI_SLIDER_FLOAT1_WITH_SETTER_FUNC(variableName, initialValue, min, max, setter)                               \
	float variableName = initialValue;                                                                                 \
	if(ImGui::SliderFloat(#variableName, &variableName, min, max)) {                                                   \
		setter(variableName);                                                                                          \
	}

#define GUI_SLIDER_FLOAT1(variableName, initialValue, min, max)                                                        \
	float variableName = initialValue;                                                                                 \
	if(ImGui::SliderFloat(#variableName, &variableName, min, max)) {                                                   \
	}

#define GUI_SLIDER_FLOAT3_WITH_SETTER_FUNC(variableName, initialValue, min, max, setter)                               \
	glm::vec3 variableName = initialValue;                                                                             \
	if(ImGui::SliderFloat3(#variableName, &variableName[0], min, max)) {                                               \
		setter(variableName)                                                                                           \
	}

#define GUI_SLIDER_FLOAT3(variableName, initialValue, min, max)                                                        \
	glm::vec3 variableName = initialValue;                                                                             \
	if(ImGui::SliderFloat3(#variableName, &variableName[0], min, max)) {                                               \
	}

#define GUI_SLIDER_QUAT(variableName, initialValue, min, max)                                                          \
	glm::quat variableName = initialValue;                                                                             \
	if(ImGui::SliderFloat4(#variableName, &variableName[0], min, max)) {                                               \
	}

// Provide a setter func which allows automatically updating some component whenever
// the value changes. For example, toggling the field of a class from outside the class
// via its getter (`initialValue`, which can be a callable object) ,and its setter (`setter`)
#define GUI_CHECKBOX_WITH_SETTER_FUNC(variableName, initialValue, setter)                                              \
	bool variableName = initialValue;                                                                                  \
	if(ImGui::Checkbox(#variableName, &variableName)) {                                                                \
		setter(variableName);                                                                                          \
	}

// Don't provide a setter func, so introspection of the value selected is only available
// from the provided `variableName` instance
#define GUI_CHECKBOX(variableName, initialValue)                                                                       \
	bool variableName = initialValue;                                                                                  \
	if(ImGui::Checkbox(#variableName, &variableName)) {                                                                \
	}
