# CMakeList which reports the list of source files required to build NimbleEngine

# Subdirectories will add on to this list of sources
set(NIMBLE_ENGINE_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/FragmentShader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/GLContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/IndexBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/Shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/ShaderProgram.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/VertexBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper/VertexShader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/window/Window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/resource-manager/ResourceManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderLoop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/Engine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GlfwRenderLoop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/input/InputManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/material/Material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph/SceneNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph/SceneGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph/RootSceneNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph/Transformation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph/DirectionalLightNode.cpp

    PARENT_SCOPE
)