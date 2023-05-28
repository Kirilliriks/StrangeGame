//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.h"

Octree::Octree() {
    maxDepth = 8;
    size = 1 << maxDepth; // std::pow(2, maxDepth);

    nodes.emplace_back(size, glm::ivec3(0, 0, 0));
}

int Octree::getSize() {
    return size;
}

void Octree::setVoxel(const glm::ivec3& vec, const glm::vec4& color) {
    setVoxel(0, 0, vec, color);
}

void Octree::setVoxel(int index, int depth, const glm::ivec3& vec, const glm::vec4& color) {
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

    const int nextIndex = currentNode.getSubNodeIndex(vec);
    setVoxel(nextIndex, ++depth, vec, color);
}

void Octree::removeVoxel(const glm::ivec3 &vec) {
    removeVoxel(0, 0, vec);
}

void Octree::removeVoxel(int index, int depth, const glm::ivec3 &vec) {
    Node currentNode = nodes.at(index);

    if (depth == maxDepth) { // If in voxel depth
        currentNode.setVoxel(glm::vec4(0, 0, 0, -1));
        nodes[index] = currentNode;
        return;
    } // else go deeper

    if (currentNode.isEmpty()) { // Hasn't voxel
        return;
    }

    const int nextIndex = currentNode.getSubNodeIndex(vec);
    removeVoxel(nextIndex, ++depth, vec);
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

Octree::DebugCast Octree::voxelRaycast(const glm::vec3& rayDirection, const glm::vec3& start_position, float maxDistance) {
    DebugCast debugCast;
    debugCast.distance = 0;
    debugCast.iterations = 0;
    debugCast.voxelPos = glm::ivec3(-1);

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
        debugCast.preVoxelPos = glm::ivec3(voxelPos);
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
            debugCast.iterations = iter;
            debugCast.distance = distance;
            debugCast.voxelPos = glm::ivec3(voxelPos);
            return debugCast;
        }
    }

    return debugCast;
}

int getSubIndexFromSubVector(const glm::ivec3& vec) {
    if (glm::min(vec.x, glm::min(vec.y, vec.z)) < 0 || glm::max(vec.x, glm::max(vec.y, vec.z)) > 1) return -1;

    int subIndex = 0;
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
    layer.subVec = glm::ivec3(layer.position) / layer.halfSize;
    layer.distance = 0.0f;
    layer.rayLength = (glm::vec3(layer.subVec + dir) * float(layer.halfSize) - layer.position) * rayStepSizeSingle;
    return layer;
}

Layer calculateLayer(Layer &layer, const glm::ivec3& dir, const glm::vec3& rayStepSizeSingle, const int halfSize) {
    layer.subVec = glm::clamp(glm::ivec3(layer.position) / halfSize, 0, 1);
    layer.rayLength = (glm::vec3(layer.subVec + dir) * float(halfSize) - layer.position) * rayStepSizeSingle;
    layer.distance = 0.0f;
    return layer;
}

struct StackItem {
    int nodeIndex;
    glm::ivec3 subVec;
};

Octree::DebugCast Octree::castDRay(const glm::vec3& rayDirection, const glm::vec3& start_position) {
    int iter = 0;
    DebugCast debugCast;
    debugCast.iterations = 0;
    debugCast.voxelPos = glm::ivec3(-1);

    const glm::vec3 rayStepSizeSingle = 1.0f / rayDirection;
    const glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));
    const glm::ivec3 dir = glm::max(step, 0);

    StackItem nodeStack[maxDepth + 1];

    Layer currentLayer;
    Node currentNode = nodes[0];
    currentLayer.nodeIndex = 0;
    currentLayer.halfSize = currentNode.halfSize;
    currentLayer.position = start_position;
    currentLayer.distance = 0.0f;
    calculateLayer(currentLayer, dir, rayStepSizeSingle);

    int currentDepth = maxDepth;
    nodeStack[currentDepth].nodeIndex = 0;
    nodeStack[currentDepth].subVec = currentLayer.subVec;

    glm::ivec3 lastStep = glm::ivec3(0);
    glm::vec3 rayStepSize = glm::vec3(currentLayer.halfSize) * glm::abs(rayStepSizeSingle);
    while (iter++ < 500) {
        int subIndex = getSubIndexFromSubVector(currentLayer.subVec);
        if (subIndex < 0) {
            while(subIndex < 0) {
                currentDepth++;
                if (currentDepth > maxDepth) {
                    debugCast.iterations = 1;
                    debugCast.voxelPos = glm::ivec3(currentLayer.position);
                    return debugCast;
                }
                rayStepSize *= 2;

                currentNode = nodes[nodeStack[currentDepth].nodeIndex];
                currentLayer.halfSize = currentNode.halfSize;
                currentLayer.subVec = nodeStack[currentDepth].subVec + lastStep;
                subIndex = getSubIndexFromSubVector(currentLayer.subVec);
            }
            continue;
        } else if (currentNode.sub != -1) {
            const int subNodeIndex = currentNode.sub + subIndex;
            currentNode = nodes[subNodeIndex];
            if (currentNode.color.a != -1.0f) {
                debugCast.voxelPos = glm::ivec3(currentNode.position);
                debugCast.iterations = iter;
                debugCast.iterations = -1;
                return debugCast;
            } else if (currentDepth != 1) { // If currentDepth == 1, this node has voxel neighbour
                currentDepth--;
                rayStepSize *= 0.5f;
                currentLayer.halfSize = currentNode.halfSize;
                currentLayer.subVec = glm::ivec3(currentLayer.position - glm::vec3(currentNode.position)) / currentLayer.halfSize;
                nodeStack[currentDepth].nodeIndex = subNodeIndex;
                nodeStack[currentDepth].subVec = currentLayer.subVec;
                continue;
            }
        }

        lastStep = glm::ivec3(0);
        if (currentLayer.rayLength.x < currentLayer.rayLength.y) {
            if (currentLayer.rayLength.x < currentLayer.rayLength.z) {
                currentLayer.distance = currentLayer.rayLength.x;
                currentLayer.rayLength.x += rayStepSize.x;
                currentLayer.subVec.x += step.x;
                lastStep.x = step.x;
            } else {
                currentLayer.distance = currentLayer.rayLength.z;
                currentLayer.rayLength.z += rayStepSize.z;
                currentLayer.subVec.z += step.z;
                lastStep.z = step.z;
            }
        } else {
            if (currentLayer.rayLength.y < currentLayer.rayLength.z) {
                currentLayer.distance = currentLayer.rayLength.y;
                currentLayer.rayLength.y += rayStepSize.y;
                currentLayer.subVec.y += step.y;
                lastStep.y = step.y;
            } else {
                currentLayer.distance = currentLayer.rayLength.z;
                currentLayer.rayLength.z += rayStepSize.z;
                currentLayer.subVec.z += step.z;
                lastStep.z = step.z;
            }
        }
        nodeStack[currentDepth].subVec = currentLayer.subVec;
        currentLayer.position = start_position + rayDirection * currentLayer.distance;
    }
    debugCast.voxelPos = glm::ivec3(currentLayer.position);
    debugCast.iterations = 2;
    return debugCast;
}

Octree::DebugCast Octree::raycastVoxel(const glm::vec3& rayDirection, const glm::vec3& start_position) {
    int iter = 0;
    DebugCast debugCast;

    const glm::vec3 rayStepSizeSingle = 1.0f / rayDirection;
    const glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));
    const glm::ivec3 dir = glm::max(step, 0);

    Layer layers[maxDepth + 1];

    int currentDepth = maxDepth - 1;
    Layer currentLayer;
    Node currentNode = nodes[0];
    currentLayer.nodeIndex = 0;
    currentLayer.position = start_position;

    int halfSize = currentNode.halfSize;
    glm::vec3 rayStepSize = glm::vec3(halfSize) * glm::abs(rayStepSizeSingle);

    layers[currentDepth] = calculateLayer(currentLayer, dir, rayStepSizeSingle, halfSize);

    float realDistance = 0.0f;
    while (iter++ < 500) {
        currentLayer = layers[currentDepth];
        currentNode = nodes[currentLayer.nodeIndex];

        const int subIndex = getSubIndexFromSubVector(currentLayer.subVec);
        if (subIndex < 0) {
            currentDepth++;
            if (currentDepth == maxDepth) return debugCast;

            rayStepSize *= 2;
            halfSize *= 2;
        } else if (currentNode.sub != -1) {
            const int subNodexIndex = currentNode.sub + subIndex;
            currentNode = nodes[subNodexIndex];

            if (currentNode.color.a != -1.0f) {
                debugCast.voxelPos = glm::ivec3(currentNode.position);
                debugCast.iterations = iter;

                for (const Layer& layer : layers) {
                    realDistance += layer.distance;
                }

                debugCast.distance = realDistance;

                return debugCast;
            }

            if (currentDepth != 0) { // If currentDepth == 0 this node empty and have voxel neighbour
                // Start subRaycast
                Layer newLayer;

                currentDepth--;
                rayStepSize /= 2;

                newLayer.nodeIndex = subNodexIndex;
                newLayer.position = currentLayer.position + currentLayer.distance * rayDirection - glm::vec3(halfSize * currentLayer.subVec);

                halfSize /= 2;

                layers[currentDepth] = calculateLayer(newLayer, dir, rayStepSizeSingle, halfSize);
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
