//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

#include <iostream>
#include <cmath>

Octree::Octree() {
    maxDepth = 6;
    size = 512; // std::pow(2, maxDepth);
    height = 128;

    //nodes.emplace_back(size, glm::ivec3(0, 0, 0));
    voxels.resize(size * size * height);
}

int Octree::getSize() {
    return size;
}

void Octree::setVoxel(const glm::ivec3& vec, glm::vec4 color) {
    setVoxel(0, 0, vec, color);
}

void Octree::setVoxel(int index, int depth, const glm::ivec3& vec, const glm::vec4& color) {
    static int zSize = height * size;
    voxels[vec.x + vec.y * height + vec.z * zSize] = color;

    return;
    Node currentNode = nodes.at(index);

    if (depth == maxDepth) { // If in voxel depth
        currentNode.setVoxel(color);
        nodes[index] = currentNode;
        return;
    } // else go deeper

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

glm::vec4 *Octree::getVData() {
    return voxels.data();
}

int Octree::nodesCount() {
    return nodes.size();
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

void moveLayer(Layer &currentLayer, const glm::ivec3 &step, const glm::vec3 &rayStepSize) {
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
}

Octree::VoxelInfo Octree::raycastVoxel(const glm::vec3& rayDirection, const glm::vec3& start_position) {
    VoxelInfo debugCast;

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

    int iter = 0;
    glm::vec3 rayStepSize = glm::vec3(currentLayer.halfSize) * glm::abs(rayStepSizeSingle);
    while (iter < 500) {
        currentLayer = layers[currentDepth];
        currentNode = nodes[currentLayer.nodeIndex];

        const int subIndex = getSubIndexFromSubVector(currentLayer.subVec);
        if (subIndex < 0) {
            currentDepth++;
            if (currentDepth > maxDepth) return debugCast;
            rayStepSize *= 2;
            moveLayer(layers[currentDepth], step, rayStepSize);
            iter++;
            continue;
        }
        iter++;

        if (currentNode.sub != -1) {
            const Node subNode = nodes[currentNode.sub + subIndex];
            if (subNode.color.a != -1.0f) {
                debugCast.voxelPos = glm::ivec3(subNode.position);
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

        moveLayer(currentLayer, step, rayStepSize);
        layers[currentDepth] = currentLayer;
    }
    return debugCast;
}
