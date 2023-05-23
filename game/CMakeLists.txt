include_directories(../dependency/glad/)
include_directories(../glfw/include)
include_directories(../glm/)
include_directories(../imgui/)

add_library(Game ../dependency/glad/glad.c Game.cpp render/shaders/Shader.cpp render/shaders/Shader.h render/VoxelRender.cpp render/VoxelRender.h render/Camera.cpp render/Camera.h render/Window.cpp render/Window.h utils/Input.cpp render/shaders/RaycastShader.cpp render/shaders/RaycastShader.h world/Octree.cpp world/Octree.h world/Node.h world/Node.cpp utils/Paths.cpp world/World.cpp world/World.h)