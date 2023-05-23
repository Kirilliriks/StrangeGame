//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_WORLD_H
#define STRANGEGAME_WORLD_H


#include "Octree.h"

class World {
public:
    World();
    ~World() = default;

    void setVoxel(const glm::ivec3& vec, const glm::vec4& color);

    Octree& getOctree();
private:
    Octree octree;
};


#endif //STRANGEGAME_WORLD_H
