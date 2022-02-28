include_directories(../dependency/glad/)
include_directories(../glfw/include)
include_directories(../glm/)

add_library(Game ../dependency/glad/glad.c Game.cpp world/Voxel.cpp world/Voxel.h utils/Color.cpp utils/Color.h render/Shader.cpp render/Shader.h render/VoxelRender.cpp render/VoxelRender.h)