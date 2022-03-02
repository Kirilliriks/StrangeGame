//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <iostream>

Octree::Octree() {
    maxDepth = 6;
    size = 1 << maxDepth; // std::pow(2, maxDepth);

    nodes.emplace_back(size, glm::ivec3(1, 0, 0));
    nodes.emplace_back(size, glm::ivec3(3, 3, 3));
    nodes.emplace_back(size, glm::ivec3(3, 3, 3));
    nodes.emplace_back(size, glm::ivec3(3, 3, 3));
}

int Octree::getSize() {
    return size;
}

void Octree::setVoxel(const glm::ivec3& vec, glm::vec4 color) {
    setVoxel(0, 0, vec, color);
}

void Octree::setVoxel(int index, int depth, const glm::ivec3& vec, glm::vec4 color) {
    Node currentNode = nodes.at(index);
    auto pos = currentNode.position;
//    std::cout << "Depth " << depth << std::endl;
//    std::cout << pos.x << "=" << vec.x << std::endl;
//    std::cout << pos.y << "=" << vec.y << std::endl;
//    std::cout << pos.z << "=" << vec.z << std::endl;

    if (depth == maxDepth) {
//        std::cout << "YAY " << std::endl;
        currentNode.setColor(color);
        return;
    }

    if (currentNode.isEmpty()) {
        currentNode.divide(nodes);
        depth++;

        nodes[index] = currentNode;
    }

    const int nextIndex = currentNode.getSubNodeIndex(vec);
    setVoxel(nextIndex, depth, vec, color);
}

Node *Octree::getData() {
    return nodes.data();
}

int Octree::nodesCount() {
    return nodes.size();
}


