//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <iostream>

Octree::Octree() {
    maxDepth = 6;
    size = 1 << maxDepth; // std::pow(2, maxDepth);

    nodes.emplace_back(size, glm::ivec3(0, 0, 0));
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
        currentNode.setVoxel(color);
        nodes[index] = currentNode;
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

//TODO сделать рабочий алгоритм!
Node Octree::getNode(const glm::ivec3& vec) {
    int index = 0;
    while(index != -1) {
        Node currentNode = nodes[index];

        std::cout << "HalfSize " << currentNode.halfSize << std::endl;
        std::cout << "X " << currentNode.position.x << std::endl;
        std::cout << "Y " << currentNode.position.y << std::endl;
        std::cout << "Z " << currentNode.position.z << std::endl;
        if (currentNode.isEmpty()) {
            if (currentNode.color.a != -1.0f) {
                std::cout << "!!!!!!!!!!!!!!!!" << std::endl;
                return currentNode;
            }
            return Node(-32, glm::ivec3(0, 0, 0));
        }

        const int subIndex = currentNode.getSubIndex(vec);
        if (subIndex < 0) {
            return Node(-16, glm::ivec3(0, 0, 0));
        }

        index = currentNode.sub + subIndex;
    }
    return Node(-8, glm::ivec3(0, 0, 0));
}


