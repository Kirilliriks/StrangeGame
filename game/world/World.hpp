//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_WORLD_H
#define STRANGEGAME_WORLD_H

#include "../render/Camera.hpp"
#include "../Game.hpp"
#include "octree/OctreeSpace.hpp"

class Game;
class World {
public:
    explicit World(Game* game);
    ~World() = default;

    void update(const double& deltaTime);
    void imgui(const double& deltaTime);
    void setVoxel(const glm::ivec3& vec, const glm::vec4& color);
    void createWorld();

    OctreeSpace &getOctreeSpace();
    Camera &getCamera();
    TraceStack &getTraceCast();
    glm::ivec3 &getFrontVoxel();

    TraceStack voxelRaycast(const glm::vec3 &rayDirection, const glm::vec3 &startPosition, float maxDistance);
private:
    Game* game;

    Camera camera;
    OctreeSpace octreeSpace = OctreeSpace(1);

    TraceStack traceCast;
    glm::ivec3 frontVoxel;
};


#endif //STRANGEGAME_WORLD_H
