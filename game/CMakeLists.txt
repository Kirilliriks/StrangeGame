include_directories(../dependency/glad/)
include_directories(../glfw/include)
include_directories(../glm/)
include_directories(../imgui/)

add_library(
        Game
        ../dependency/glad/gl.c ../dependency/ogt/ogt_vox.h
        Game.cpp
        render/shaders/Shader.cpp render/shaders/Shader.hpp
        render/voxel/VoxelRender.cpp render/voxel/VoxelRender.hpp
        render/Camera.cpp render/Camera.hpp
        render/Window.cpp render/Window.hpp
        utils/Input.hpp
        render/shaders/RaycastShader.cpp render/shaders/RaycastShader.hpp
        world/octree/Octree.cpp world/octree/Octree.hpp
        world/octree/Node.hpp world/octree/Node.cpp
        world/World.cpp world/World.hpp
        utils/Input.cpp
        render/polygon/PolygonRender.cpp
        render/polygon/PolygonRender.hpp
        render/polygon/PolygonRender.hpp
        render/polygon/mesh/Mesh.cpp
        render/polygon/mesh/Mesh.hpp
        render/polygon/mesh/MeshBuilder.cpp
        render/polygon/mesh/MeshBuilder.hpp
        render/polygon/object/Object.cpp
        render/polygon/object/Object.hpp
        render/polygon/mesh/MeshStorage.cpp
        render/polygon/mesh/MeshStorage.hpp
        world/octree/TraceStack.hpp
        world/octree/OctreeSpace.cpp
        world/octree/OctreeSpace.hpp
        options/Options.hpp
        options/Options.cpp
)