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
	'RenderLoop.cpp',
	'engine/Engine.cpp',
	'GlfwRenderLoop.cpp',
	'input/InputManager.cpp',
	'material/Material.cpp',
	'camera/Camera.cpp',
    'scene-graph/SceneNode.cpp',
    'scene-graph/SceneGraph.cpp',
    'scene-graph/RootSceneNode.cpp',
    'scene-graph/Transformation.cpp',
    'scene-graph/DirectionalLightNode.cpp',

	# imgui
	'thirdparty/imgui/src/imgui_demo.cpp',
	'thirdparty/imgui/src/imgui_draw.cpp',
	'thirdparty/imgui/src/imgui_widgets.cpp',
	'thirdparty/imgui/src/imgui.cpp',
	'thirdparty/imgui/src/imgui_impl_glfw.cpp',
	'thirdparty/imgui/src/imgui_impl_opengl3.cpp',
] 

include_dir = include_directories('../include', './thirdparty/imgui/include')

#PROJECT DEPENDS
OpenGLDepend = dependency('GL', required: true) 
fmtLibDepend = subproject('fmt').get_variable('fmt_dep') 
GLEWDepend = dependency('glew', required : true) 
GLFWDepend = dependency('glfw3', required: true)
SpdLogDepend = subproject('spdlog').get_variable('spdlog_dep')
GlmDepend = dependency('glm', required: true)
AssimpDepend = dependency('assimp', required: true)


ng = shared_library('NimbleGraphics', sources: src, include_directories : include_dir, 
					dependencies : [OpenGLDepend, GLEWDepend, GLFWDepend, fmtLibDepend, SpdLogDepend, GlmDepend, AssimpDepend])

NimbleGraphicsDepend = declare_dependency(link_with: ng, include_directories: include_dir, dependencies: [OpenGLDepend, GLEWDepend, GLFWDepend, fmtLibDepend, SpdLogDepend, GlmDepend, AssimpDepend])
