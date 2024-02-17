include_directories(../dependency/glad/)
include_directories(../glfw/include)
include_directories(../glm/)
include_directories(../imgui/)

add_library(
        Game ../dependency/glad/glad.c Game.cpp
        render/shaders/Shader.cpp render/shaders/Shader.hpp
        render/voxel/VoxelRender.cpp render/voxel/VoxelRender.hpp
        render/Camera.cpp render/Camera.hpp
        render/Window.cpp render/Window.hpp
        utils/Input.hpp
        render/shaders/RaycastShader.cpp render/shaders/RaycastShader.hpp
        world/Octree.cpp world/Octree.hpp
        world/Node.hpp world/Node.cpp
        utils/Paths.hpp
        world/World.cpp world/World.hpp
        utils/Input.cpp
        render/polygon/PolygonRender.cpp
        render/polygon/PolygonRender.hpp
        render/polygon/PolygonRender.hpp
)