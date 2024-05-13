//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_TESTUNG_H
#define STRANGEGAME_TESTUNG_H

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "docktest.h"
#include "../game/world/octree/Octree.hpp"

TEST_CASE("Octree instancing") {
    CHECK(Octree(8).nodesCount() == 1);
}

TEST_CASE("Octree set voxel") {
    Octree octree(8);
    octree.setVoxel(glm::ivec3(0), glm::vec4(255));
    CHECK(octree.getVoxel(glm::ivec3(0)).color == glm::vec4(255));
}

TEST_CASE("Octree remove voxel") {
    Octree octree(8);
    octree.setVoxel(glm::ivec3(0), glm::vec4(255));
    octree.removeVoxel(glm::ivec3(0));
    CHECK(octree.getVoxel(glm::ivec3(0)).color != glm::vec4(255));
}
#endif //STRANGEGAME_TESTUNG_H
