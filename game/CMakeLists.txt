include_directories(../dependency/glad/)
include_directories(../glfw/include)
include_directories(../glm/)

add_library(Game ../dependency/glad/glad.c Game.cpp world/Voxel.cpp world/Voxel.h utils/Color.cpp utils/Color.h render/shaders/Shader.cpp render/shaders/Shader.h render/VoxelRender.cpp render/VoxelRender.h render/Camera.cpp render/Camera.h render/Window.cpp render/Window.h utils/Input.cpp render/shaders/RaycastShader.cpp render/shaders/RaycastShader.h)