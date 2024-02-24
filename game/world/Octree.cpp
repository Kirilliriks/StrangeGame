//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.hpp"

Octree::Octree() {
    maxDepth = 5; // OLD 8
    size = 1 << maxDepth; // std::pow(2, maxDepth);

    nodes.emplace_back(size, glm::ivec3(0));
}

int Octree::getSize() const {
    return size;
}

void Octree::setVoxel(const glm::ivec3& vec, const glm::vec4& color) {
    setVoxel(0, 0, vec, color);
}

Node Octree::getVoxel(const glm::ivec3& vec) {
    return getVoxel(0, 0, vec);
}

void Octree::setVoxel(const int& index, const int& depth, const glm::ivec3& vec, const glm::vec4& color) {
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
    setVoxel(nextIndex, depth + 1, vec, color);
}

Node Octree::getVoxel(const int& index, const int& depth, const glm::ivec3& vec) {
    Node currentNode = nodes.at(index);

    if (depth == maxDepth) { // If in voxel depth
        nodes[index] = currentNode;
        return currentNode;
    } // else go deeper

    if (currentNode.isEmpty()) { // Hasn't voxel
        return {-1, glm::ivec3(-1)};
    }

    const int nextIndex = currentNode.getSubNodeIndex(vec);
    return getVoxel(nextIndex, depth + 1, vec);
}

void Octree::removeVoxel(const glm::ivec3 &vec) {
    removeVoxel(0, 0, vec);
}

void Octree::removeVoxel(const int& index, const int& depth, const glm::ivec3 &vec) {
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
    removeVoxel(nextIndex, depth + 1, vec);
}

Node *Octree::getData() {
    return nodes.data();
}

int Octree::nodesCount() const {
    return nodes.size();
}

int getSubIndexFromVector(const glm::ivec3& vec, const Node& node) {
    const auto pos = glm::ivec3(node.position);
    if (vec.x < pos.x) return -1;
    if (vec.y < pos.y) return -2;
    if (vec.z < pos.z) return -3;

    const int halfSize = node.halfSize;
    const int size = halfSize * 2;
    if (vec.x > pos.x + size) return -4;
    if (vec.y > pos.y + size) return -5;
    if (vec.z > pos.z + size) return -6;

    int subIndex = 0;
    subIndex |= vec.x >= pos.x + halfSize? 2 : 0;
    subIndex |= vec.y >= pos.y + halfSize? 4 : 0;
    subIndex |= vec.z >= pos.z + halfSize? 1 : 0;

    return subIndex;
}

int Octree::findVoxel(const glm::ivec3& voxelPos) const {
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

TraceStack Octree::voxelRaycastDDA(const glm::vec3& rayDirection, const glm::vec3& start_position, float maxDistance) const {
    TraceStack traceStack;
    traceStack.distance = 0;
    traceStack.iterations = 0;
    traceStack.voxelPos = glm::ivec3(-1);

    const auto dir = glm::ivec3(rayDirection.x > 0.0f, rayDirection.y > 0.0f, rayDirection.z > 0.0f);
    const auto step = glm::ivec3(glm::sign(rayDirection));
    const glm::vec3 rayStepSize = glm::abs(glm::vec3(1.0f) / rayDirection);

    auto voxelPos = glm::ivec4(start_position, 0);
    auto rayLength = (glm::vec3(glm::ivec3(start_position) + dir) - start_position) / rayDirection;

    int iter = 0;
    float distance = 0.0f;
    while(distance < maxDistance) {
        traceStack.entryStack.emplace_back(start_position + distance * rayDirection);

        iter++;
        traceStack.preVoxelPos = glm::ivec3(voxelPos);
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
            traceStack.iterations = iter;
            traceStack.distance = distance;
            traceStack.voxelPos = glm::ivec3(voxelPos);
            return traceStack;
        }
    }

    return traceStack;
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
    int nodeIndex = -1;
    int halfSize = -1;
    glm::vec3 position;
    glm::ivec3 subVec;
    glm::vec3 rayLength;

    float distance = 0.0f;
};

Layer calculateLayer(Layer &layer, const glm::ivec3& dir, const glm::vec3& rayStepSizeSingle, const int halfSize) {
    layer.subVec = glm::clamp(glm::ivec3(layer.position) / halfSize, 0, 1);
    layer.rayLength = (glm::vec3(layer.subVec + dir) * static_cast<float>(halfSize) - layer.position) * rayStepSizeSingle;
    layer.distance = 0.0f;
    return layer;
}

TraceStack Octree::voxelRaycastTraversal(const glm::vec3& rayDirection, const glm::vec3& start_position) const {
    int iter = 0;
    TraceStack traceStack;

    const glm::vec3 rayStepSizeSingle = 1.0f / rayDirection;
    const auto step = glm::ivec3(glm::sign(rayDirection));
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
        traceStack.nodesStack.push_back(currentNode);

        if (const int subIndex = getSubIndexFromSubVector(currentLayer.subVec); subIndex < 0) {
            currentDepth++;
            if (currentDepth == maxDepth) return traceStack;

            rayStepSize *= 2;
            halfSize *= 2;
        } else if (currentNode.sub != -1) {
            const int subNodexIndex = currentNode.sub + subIndex;
            currentNode = nodes[subNodexIndex];

            if (currentNode.color.a != -1.0f) {
                traceStack.voxelPos = glm::ivec3(currentNode.position);
                traceStack.iterations = iter;

                for (const Layer& layer : layers) {
                    realDistance += layer.distance;
                    traceStack.entryStack.emplace_back(start_position + realDistance * rayDirection);
                }

                traceStack.distance = realDistance;

                return traceStack;
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
    return traceStack;
}
