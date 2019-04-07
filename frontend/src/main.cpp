// --- Dont move this, its stopping clang format from reordering includes
// --- and glad _must_ come before glfw
#include <glad/glad.h>
///
#include <GLFW/glfw3.h>
#include <array>
#include <fmt/format.h>
#include <iostream>

#include "nimble/Mesh.h"
#include "nimble/MeshTools.h"
#include "nimble/opengl-wrapper/GLContext.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"
#include "nimble/utility/FileUtility.h"
#include "nimble/window/Window.h"
#include "nimble/opengl-wrapper/VertexArrayObject.h"

#include <filesystem>

using std::cout;
using std::endl;

using namespace Nimble;

void glfw_error_callback(int error, const char *description) {
	std::cout << fmt::format("GLFW Error # {}: {}", error, description).c_str() << endl;
}

// Do all required startup operations like library initialization
// early debug info, etc.
// Any failures result in a debug output + exit
void start_up() {
	if(!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << endl;
		exit(-1);
	}
}

void clean_up() {
	glfwTerminate();
}

int main() {
	std::cout << std::filesystem::current_path() << std::endl;
	try {
		start_up();
		// main application code here

		Window w(Width(1920), Height(1080), "Test Title");

		// Cannot initialize GLAD until we've set the context, which happens in Window
		if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			std::cout << "Failed to initialize GLAD" << endl;
			throw std::runtime_error("Failed to initialize GLAD");
		}

		const auto windowPointer = w.GetWindow();

		auto mesh = MeshTools::CreateTriangle();

		auto *indexBuffer = new IndexBuffer(mesh.NumIndices(), BufferUsageType::Static);
		indexBuffer->SetData(mesh.IndexData());

		auto *vertexBuffer = new VertexBuffer<Position>(mesh.VertexData(), BufferUsageType::Static);

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		ShaderProgram program;
		program.AddVertexShader(FileReadAllText("resources\\shaders\\basic.vert").c_str());
		program.AddFragmentShader(FileReadAllText("resources\\shaders\\basic.frag").c_str());
		if(!program.LinkShaders()) {
			throw std::runtime_error("Failed to link shaders!");
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// render loop
		while(!glfwWindowShouldClose(windowPointer)) {
			glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			program.Use();
			vertexBuffer->Bind();
			indexBuffer->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(windowPointer);
			glfwPollEvents();
		}
	} catch(std::exception &ex) {
		std::cout << "Unhandled exception with message: " << ex.what() << std::endl;
		clean_up();
		char blah;
		std::cin >> blah;
		return -1;
	}

	clean_up();

	return 0;
}