//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_TESTUNG_H
#define STRANGEGAME_TESTUNG_H

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "docktest.h"
#include "../game/world/octree/Octree.hpp"
#include "../game/world/octree/OctreeSpace.hpp"

TEST_CASE("Node instancing") {
    Node node(glm::ivec3(0));
    CHECK(node.isEmpty());
    CHECK(node.isEmptyVoxel());
}

TEST_CASE("Node get sub index") {
    const Node node(glm::ivec3(0));
    CHECK(node.getSubIndex(2, glm::ivec3(0, 0, 0)) == 0);
    CHECK(node.getSubIndex(2, glm::ivec3(0, 0, 2)) == 1);
    CHECK(node.getSubIndex(2, glm::ivec3(2, 0, 0)) == 2);
    CHECK(node.getSubIndex(2, glm::ivec3(2, 0, 2)) == 3);
    CHECK(node.getSubIndex(2, glm::ivec3(0, 2, 0)) == 4);
    CHECK(node.getSubIndex(2, glm::ivec3(0, 2, 2)) == 5);
    CHECK(node.getSubIndex(2, glm::ivec3(2, 2, 0)) == 6);
    CHECK(node.getSubIndex(2, glm::ivec3(2, 2, 2)) == 7);
}

TEST_CASE("Octree instancing") {
    const Octree octree(8);
    CHECK(octree.nodesCount() == 1);
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

TEST_CASE("OctreeSpace instancing") {
    const OctreeSpace octree(0);
    CHECK(octree.getOctreeSideSize() == 1 << 8);
    CHECK(octree.getDiameter() == 1);
}
#endif //STRANGEGAME_TESTUNG_H
