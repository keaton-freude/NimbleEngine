#Renderer Source
src = [
	'Mesh.cpp', 
	'opengl-wrapper/FragmentShader.cpp', 
	'opengl-wrapper/GLContext.cpp',
	'opengl-wrapper/IndexBuffer.cpp',
	'opengl-wrapper/Shader.cpp', 
	'opengl-wrapper/ShaderProgram.cpp',
	'opengl-wrapper/VertexBuffer.cpp', 
	'opengl-wrapper/VertexShader.cpp', 
	'window/Window.cpp', 
	'resource-manager/ResourceManager.cpp',
	'BasicShader.cpp',
	'RenderLoop.cpp',
	'engine/Engine.cpp',
	'GlfwRenderLoop.cpp'
] 

include_dir = include_directories('../include')

#PROJECT DEPENDS
OpenGLDepend = dependency('GL', required: true) 
fmtLibDepend = subproject('fmt').get_variable('fmt_dep') 
GLEWDepend = dependency('glew', required : true) 
GLFWDepend = dependency('glfw3', required: true)

ng = shared_library('NimbleGraphics', sources: src, include_directories : include_dir, 
					version: '0.0.1', soversion: '0', 
					dependencies : [OpenGLDepend, GLEWDepend, GLFWDepend, fmtLibDepend])

NimbleGraphicsDepend = declare_dependency(link_with: ng, include_directories: include_dir)
