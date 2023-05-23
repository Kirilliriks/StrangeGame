//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_WORLD_H
#define STRANGEGAME_WORLD_H

#include <glad.h>
#include "GLFW/glfw3.h"
#include "Octree.h"
#include "../render/Camera.h"
#include "../Game.h"

class Game;
class World {
public:
    World(Game* game);
    ~World() = default;

    void update(double deltaTime);
    void setVoxel(const glm::ivec3& vec, const glm::vec4& color);
    void createWorld();

    Octree &getOctree();
    Camera &getCamera();
    Octree::DebugCast &getDebugCast();
    glm::ivec3 &getFrontVoxel();
private:
    Game* game;

    Camera camera;
    Octree octree;

    Octree::DebugCast debugCast;
    glm::ivec3 frontVoxel;
};


#endif //STRANGEGAME_WORLD_H
