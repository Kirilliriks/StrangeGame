//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <cmath>
#include <iostream>

Octree::Octree() {
    const int halfSizeInVoxels = std::pow(4, maxDepth) / 2;
    nodes.emplace_back(0, halfSizeInVoxels * 2, glm::ivec3(halfSizeInVoxels, halfSizeInVoxels, halfSizeInVoxels)); // TODO maybe only glm::vec3(number) ?
}

void Octree::setVoxel(const glm::ivec3& vec, const Color &color) {
    setVoxel(0, 0, vec, color);
}

void Octree::setVoxel(int index, int depth, const glm::ivec3& vec, const Color &color) {
    Node currentNode = nodes.at(index);
    auto pos = currentNode.getPosition();
    std::cout << pos.x << "=" << vec.x << std::endl;
    std::cout << pos.y << "=" << vec.y << std::endl;
    std::cout << pos.z << "=" << vec.z << std::endl;

    if (depth == maxDepth) {
        std::cout << "YAY " << std::endl;
        currentNode.setColor(color);
        return;
    }

    if (currentNode.isEmpty()) {
        currentNode.divide(nodes);
        depth++;
    }

    const int subIndex = currentNode.getSubIndex(vec);
    const int nextIndex = currentNode.getSubNodeIndex(subIndex);
    setVoxel(nextIndex, depth, vec, color);
}


