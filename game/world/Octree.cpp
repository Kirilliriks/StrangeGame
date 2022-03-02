//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <iostream>

Octree::Octree() {
    maxDepth = 6;
    size = 1 << maxDepth; // std::pow(2, maxDepth);

    nodes.emplace_back(8, glm::ivec3(0, 0, 0));
}

int Octree::getSize() {
    return size;
}

void Octree::setVoxel(const glm::ivec3& vec, glm::vec4 color) {
    setVoxel(0, 0, vec, color);
}

void Octree::setVoxel(int index, int depth, const glm::ivec3& vec, glm::vec4 color) {
    Node currentNode = nodes.at(index);

    if (depth == maxDepth) {
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


