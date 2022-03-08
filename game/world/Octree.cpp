//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <iostream>
#include <cmath>

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

    if (depth == maxDepth) { // If in voxel depth
        currentNode.setVoxel(color);
        nodes[index] = currentNode;
        return;
    } // else go deeper, AND INCREMENT DEPTH!!!

    if (currentNode.isEmpty()) {
        currentNode.divide(nodes);
        nodes[index] = currentNode;
    }
    depth++;

    const int nextIndex = currentNode.getSubNodeIndex(vec);
    setVoxel(nextIndex, depth, vec, color);
}

Node *Octree::getData() {
    return nodes.data();
}

int Octree::nodesCount() {
    return nodes.size();
}

int getSubIndexFromVector(const glm::ivec3& vec, const Node& node) {
    int subIndex = 0;

    const glm::ivec3 pos = glm::ivec3(node.position);
    if (vec.x < pos.x) return -1;
    if (vec.y < pos.y) return -2;
    if (vec.z < pos.z) return -3;

    const int halfSize = node.halfSize;
    const int size = halfSize * 2;
    if (vec.x > pos.x + size) return -4;
    if (vec.y > pos.y + size) return -5;
    if (vec.z > pos.z + size) return -6;

    subIndex |= vec.x >= pos.x + halfSize? 2 : 0;
    subIndex |= vec.y >= pos.y + halfSize? 4 : 0;
    subIndex |= vec.z >= pos.z + halfSize? 1 : 0;

    return subIndex;
}

int Octree::findVoxel(const glm::ivec3& voxelPos) {
    int index = 0;
    while(index != -1) {
        const Node currentNode = nodes[index];
        if (currentNode.sub == -1) {
            if (currentNode.color.a != -1.0f) {
                return 1;
            }
            return 2;
        }

        const int subIndex = getSubIndexFromVector(voxelPos, currentNode);
        if (subIndex < 0) return -1;

        index = currentNode.sub + subIndex;
    }
    return -1;
}

glm::ivec4 Octree::voxelRaycast(const glm::vec3& rayDirection, const glm::vec3& start_position, float maxDistance) {
    float distance = 0.0f;

    const glm::vec3 rayStepSize = glm::abs(glm::vec3(1.0f) / rayDirection);

    glm::vec3 rayLength;
    glm::ivec4 voxelPos = glm::ivec4(start_position, 0);
    glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));

    glm::ivec3 dir = glm::ivec3(rayDirection.x > 0.0f, rayDirection.y > 0.0f, rayDirection.z > 0.0f);
    rayLength = (glm::vec3(glm::ivec3(start_position) + dir) - start_position) / rayDirection;

    int iter = 0;
    while(distance < maxDistance) {
        iter++;
        if (rayLength.x < rayLength.y) {
            if (rayLength.x < rayLength.z) {
                voxelPos.x += step.x;
                distance = rayLength.x;
                rayLength.x += rayStepSize.x;
            } else {
                voxelPos.z += step.z;
                distance = rayLength.z;
                rayLength.z += rayStepSize.z;
            }
        } else {
            if (rayLength.y < rayLength.z) {
                voxelPos.y += step.y;
                distance = rayLength.y;
                rayLength.y += rayStepSize.y;
            } else {
                voxelPos.z += step.z;
                distance = rayLength.z;
                rayLength.z += rayStepSize.z;
            }
        }

        if (findVoxel(glm::ivec3(voxelPos)) == 1) {
            voxelPos.w = iter;
            return voxelPos;
        }
    }
    return glm::ivec4(-1, -1, -1, 0);
}

int getSubIndexFromSubVector(const glm::ivec3& vec) {
    int subIndex = 0;

    if (vec.x < 0) return -1;
    if (vec.y < 0) return -2;
    if (vec.z < 0) return -3;


    if (vec.x > 1) return -4;
    if (vec.y > 1) return -5;
    if (vec.z > 1) return -6;

    subIndex |= (vec.x >= 1 ? 2 : 0);
    subIndex |= (vec.y >= 1 ? 4 : 0);
    subIndex |= (vec.z >= 1 ? 1 : 0);

    return subIndex;
}

struct Layer {
    int nodeIndex;
    int halfSize;
    glm::vec3 position;
    glm::ivec3 subVec;
    glm::vec3 rayLength;

    float distance = 0.0f;
};

Layer calculateLayer(Layer &layer, const glm::ivec3& dir, const glm::vec3& rayStepSizeSingle) {
    layer.subVec = glm::min(glm::ivec3(layer.position) / layer.halfSize, 1);
    layer.distance = 0.0f;
    layer.rayLength = (glm::vec3(layer.subVec + dir) * float(layer.halfSize) - layer.position) * rayStepSizeSingle;
    return layer;
}

Octree::DebugCast Octree::raycastVoxel(const glm::vec3& rayDirection, const glm::vec3& start_position) {
    int iter = 0;
    DebugCast debugCast;

    const glm::vec3 rayStepSizeSingle = 1.0f / rayDirection;
    const glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));
    const glm::ivec3 dir = glm::max(step, 0);

    Layer layers[maxDepth + 1];
    Layer currentLayer;
    Node currentNode = nodes[0];
    currentLayer.nodeIndex = 0;
    currentLayer.halfSize = currentNode.halfSize;
    currentLayer.position = start_position;
    currentLayer.distance = 0.0f;
    layers[maxDepth] = calculateLayer(currentLayer, dir, rayStepSizeSingle);

    int currentDepth = maxDepth;
    float realDistance = 0.0f;

    bool skip = false;
    glm::vec3 rayStepSize = glm::vec3(currentLayer.halfSize) * glm::abs(rayStepSizeSingle);
    while (iter++ < 500) {
        currentLayer = layers[currentDepth];
        currentNode = nodes[currentLayer.nodeIndex];

        const int subIndex = getSubIndexFromSubVector(currentLayer.subVec);
        if (subIndex < 0) {
            currentDepth++;
            if (currentDepth > maxDepth) return debugCast;
            rayStepSize *= 2;
        } else if (currentNode.sub != -1) {
            const Node subNode = nodes[currentNode.sub + subIndex];
            if (subNode.color.a != -1.0f) {
                debugCast.voxelPos = glm::ivec3(subNode.position);
                debugCast.iterations = iter;
                return debugCast;
            } else if (currentDepth != 1) { // If currentDepth == 1 this node have voxel neighbour,
                currentDepth--;
                rayStepSize *= 0.5f;

                // Start subRaycast
                Layer newLayer;
                newLayer.nodeIndex = currentNode.sub + subIndex;
                newLayer.halfSize = subNode.halfSize;
                newLayer.position = currentLayer.position + currentLayer.distance * rayDirection - glm::vec3(currentLayer.halfSize * currentLayer.subVec);
                newLayer.distance = 0.0f;
                layers[currentDepth] = calculateLayer(newLayer, dir, rayStepSizeSingle);

                realDistance += currentLayer.distance;
                continue;
            }
        }

        currentLayer = layers[currentDepth];
        if (currentLayer.rayLength.x < currentLayer.rayLength.y) {
            if (currentLayer.rayLength.x < currentLayer.rayLength.z) {
                currentLayer.distance = currentLayer.rayLength.x;
                currentLayer.rayLength.x += rayStepSize.x;
                currentLayer.subVec.x += step.x;
            } else {
                currentLayer.distance = currentLayer.rayLength.z;
                currentLayer.rayLength.z += rayStepSize.z;
                currentLayer.subVec.z += step.z;
            }
        } else {
            if (currentLayer.rayLength.y < currentLayer.rayLength.z) {
                currentLayer.distance = currentLayer.rayLength.y;
                currentLayer.rayLength.y += rayStepSize.y;
                currentLayer.subVec.y += step.y;
            } else {
                currentLayer.distance = currentLayer.rayLength.z;
                currentLayer.rayLength.z += rayStepSize.z;
                currentLayer.subVec.z += step.z;
            }
        }
        layers[currentDepth] = currentLayer;
    }
    return debugCast;
}