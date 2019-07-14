// Helper OpenGL functioons/classes/etc

#pragma once

#include "GL/glew.h"
#include <spdlog/spdlog.h>
#include <string>

// Add additional enums as we need them (or maybe there's a table somewhere??)
static inline std::string GLenumToString(GLenum type) {
	// Quick hack to get some common typenames working

	std::string typeName = "";

	if(type == GL_FLOAT_VEC2) {
		typeName = "vec2f";
	} else if(type == GL_FLOAT_VEC3) {
		typeName = "vec3f";
	} else if(type == GL_FLOAT_VEC4) {
		typeName = "vec4f";
	} else if(type == GL_INT_VEC2) {
		typeName = "vec2i";
	} else if(type == GL_INT_VEC3) {
		typeName = "vec3i";
	} else if(type == GL_INT_VEC4) {
		typeName = "vec4i";
	} else if(type == GL_FLOAT_MAT2) {
		typeName = "mat2x2f";
	} else if(type == GL_FLOAT_MAT2x3) {
		typeName = "mat2x3f";
	} else if(type == GL_FLOAT_MAT2x4) {
		typeName = "mat2x4f";
	} else if(type == GL_FLOAT_MAT3) {
		typeName = "mat3x3f";
	} else if(type == GL_FLOAT_MAT3x2) {
		typeName = "mat3x2f";
	} else if(type == GL_FLOAT_MAT3x4) {
		typeName = "mat3x4f";
	} else if(type == GL_FLOAT_MAT4) {
		typeName = "mat4x4f";
	} else if(type == GL_FLOAT_MAT4x2) {
		typeName = "mat4x2f";
	} else if(type == GL_FLOAT_MAT4x3) {
		typeName = "mat4x3f";
	} else {
		spdlog::warn("GLenum Type -> String, no entry for value {:x}", type);
		typeName = "unknown";
	}

	return typeName;
}