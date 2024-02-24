//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_WORLD_H
#define STRANGEGAME_WORLD_H

#include "Octree.hpp"
#include "../render/Camera.hpp"
#include "../Game.hpp"

class Game;
class World {
public:
    explicit World(Game* game);
    ~World() = default;

    void update(const double& deltaTime);
    void imgui(const double& deltaTime) const;
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
