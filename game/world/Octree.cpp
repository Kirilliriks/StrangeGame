//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <cmath>

Octree::Octree() {
    const int halfSizeInVoxels = std::pow(2, maxDepth) / 2;
    nodes.emplace_back(0, halfSizeInVoxels * 2, glm::ivec3(halfSizeInVoxels - 1, halfSizeInVoxels - 1, halfSizeInVoxels - 1)); // TODO maybe only glm::vec3(number) ?
}

void Octree::setVoxel(const glm::ivec3& vec, const Color &color) {
    setVoxel(0, 0, vec, color);
}

void Octree::setVoxel(int index, int depth, const glm::ivec3& vec, const Color &color) {
    Node currentNode = nodes.at(index);
    auto pos = currentNode.getPosition();

    if (depth == maxDepth) {
        currentNode.setColor(color);
        return;
    }

    if (currentNode.isEmpty()) {
        currentNode.divide(nodes);
        depth++;
    }

    const int nextIndex = currentNode.getSubNodeIndex(vec);
    setVoxel(nextIndex, depth, vec, color);
}


