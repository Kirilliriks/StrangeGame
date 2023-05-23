//
// Created by KirilliriK on 23.05.2023.
//

#include "World.h"

World::World() {

}

void World::setVoxel(const glm::ivec3 &vec, const glm::vec4& color) {
    octree.setVoxel(vec, color);
}


Octree &World::getOctree() {
    return octree;
}