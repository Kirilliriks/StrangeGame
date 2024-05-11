//
// Created by KirilliriK on 01.03.2022.
//

#include "Octree.hpp"

#include <iostream>
#include <random>

Octree::Octree(const int& maxDepth) : maxDepth(maxDepth) {
    nodes.emplace_back(glm::ivec3(0));
}

void Octree::setVoxel(const glm::ivec3& vec, const glm::vec4& color) {
    setVoxel(0, 0, vec, color);
}

Node Octree::getVoxel(const glm::ivec3& vec) {
    return getVoxel(0, 0, vec);
}

Node Octree::getNode(const glm::ivec3& vec, const int depth) const {
    return getNode(0, 0, depth, vec);
}

void Octree::divide(const int& halfSize, Node& currentNode) {
    currentNode.sub = nodes.size();
    for (int i = 0; i < 8; i++) {
        auto pos = glm::ivec3(currentNode.position);

        if ((i & 2) == 2) {
            pos.x += halfSize;
        }

        if ((i & 4) == 4) {
            pos.y += halfSize;
        }

        if ((i & 1) == 1) {
            pos.z += halfSize;
        }

        nodes.emplace_back(pos);
    }
}

void Octree::setVoxel(const int& index, const int& depth, const glm::ivec3& vec, const glm::vec4& color) {
    Node currentNode = nodes.at(index);

    if (depth == maxDepth) { // If in voxel depth
        currentNode.setVoxel(color);
        nodes[index] = currentNode;
        return;
    } // else go deeper

    const int halfSize = (1 << maxDepth - depth) / 2;
    if (currentNode.isEmpty()) {
        divide(halfSize, currentNode);
        nodes[index] = currentNode;
    }

    const int nextIndex = currentNode.getSubNodeIndex(halfSize, vec);
    setVoxel(nextIndex, depth + 1, vec, color);
}

Node Octree::getVoxel(const int& index, const int& depth, const glm::ivec3& vec) const {
    return getNode(index, depth + 1, maxDepth, vec);
}

Node Octree::getNode(const int& index, const int& depth, const int& nodeDepth, const glm::ivec3& vec) const {
    Node currentNode = nodes[index];

    if (depth == nodeDepth) {
        return currentNode;
    } // else go deeper

    if (currentNode.isEmpty()) { // Hasn't voxel
        return {glm::ivec3(-1)};
    }

    const int nextIndex = currentNode.getSubNodeIndex((1 << maxDepth - depth) / 2, vec);
    return getNode(nextIndex, depth + 1, nodeDepth, vec);
}

void Octree::removeVoxel(const glm::ivec3& vec) {
    removeVoxel(0, 0, vec);
}

void Octree::removeVoxel(const int& index, const int& depth, const glm::ivec3& vec) {
    Node currentNode = nodes.at(index);

    if (depth == maxDepth) { // If in voxel depth
        currentNode.setVoxel(glm::vec4(0, 0, 0, -1));
        nodes[index] = currentNode;
        return;
    } // else go deeper

    if (currentNode.isEmpty()) { // Hasn't voxel
        return;
    }

    const int nextIndex = currentNode.getSubNodeIndex((1 << maxDepth - depth) / 2, vec);
    removeVoxel(nextIndex, depth + 1, vec);
}

Node* Octree::getData() {
    return nodes.data();
}

std::vector<Node>& Octree::getDataVector() {
    return nodes;
}

int Octree::nodesCount() const {
    return nodes.size();
}

int getSubIndexFromVector(const int& halfSize, const glm::ivec3& vec, const Node& node) {
    const auto pos = glm::ivec3(node.position);
    if (vec.x < pos.x) return -1;
    if (vec.y < pos.y) return -2;
    if (vec.z < pos.z) return -3;

    const int size = halfSize * 2;
    if (vec.x > pos.x + size) return -4;
    if (vec.y > pos.y + size) return -5;
    if (vec.z > pos.z + size) return -6;

    int subIndex = 0;
    subIndex |= vec.x >= pos.x + halfSize ? 2 : 0;
    subIndex |= vec.y >= pos.y + halfSize ? 4 : 0;
    subIndex |= vec.z >= pos.z + halfSize ? 1 : 0;

    return subIndex;
}

int Octree::findVoxel(const glm::ivec3& voxelPos) const {
    int index = 0;

    int halfSize = (1 << maxDepth) / 2;
    while (index != -1) {
        const Node currentNode = nodes[index];
        if (currentNode.sub == -1) {
            if (currentNode.color.a != -1.0f) {
                return 1;
            }
            return 2;
        }

        const int subIndex = getSubIndexFromVector(halfSize, voxelPos, currentNode);
        if (subIndex < 0) return -1;

        index = currentNode.sub + subIndex;
        halfSize /= 2;
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

    auto voxelPos = glm::ivec3(start_position);
    auto rayLength = (glm::vec3(voxelPos + dir) - start_position) / rayDirection;

    int iter = 0;
    float distance = 0.0f;
    while (distance < maxDistance) {
        traceStack.entryStack.emplace_back(start_position + distance * rayDirection);

        iter++;
        traceStack.preVoxelPos = voxelPos;
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

        if (findVoxel(voxelPos) == 1) {
            traceStack.iterations = iter;
            traceStack.distance = distance;
            traceStack.voxelPos = voxelPos;
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

glm::vec3 insideCubeHit(
    const glm::vec3& localRayPosition,
    const glm::vec3& rayDirection,
    const glm::vec3& rayStepSizeSingle,
    float size
) {
    size *= 0.5;

    return -(glm::sign(rayDirection) * (localRayPosition - size) - size) * rayStepSizeSingle;
}

TraceStack Octree::voxelRaycastTraversal(const glm::vec3& rayDirection, const glm::vec3& rayStartPosition, const glm::vec3& octreePosition) const {
    TraceStack traceStack;

    const glm::vec3 rayStepSizeSingle = 1.0f / glm::max(glm::abs(rayDirection), 0.001f);

    float size = (1 << maxDepth);

    glm::vec3 localRayPosition = rayStartPosition - octreePosition;
    glm::vec3 voxelRayPosition = glm::vec3(0);

    //std::cout << "RSP x=" << rayStartPosition.x << " y=" << rayStartPosition.y << " z=" << rayStartPosition.z << std::endl;
    //std::cout << "LRP x=" << localRayPosition.x << " y=" << localRayPosition.y << " z=" << localRayPosition.z << std::endl;
    //std::cout << "VRP x=" << voxelRayPosition.x << " y=" << voxelRayPosition.y << " z=" << voxelRayPosition.z << std::endl;

    glm::vec3 mask;
    bool exitOctree = false;
    int depth = 0;
    float distance = 0.0;

    int iterations;
    for (iterations = 0; iterations < 100 && distance < 200; iterations++) {

        if (exitOctree) {
            size *= 2.0;
            glm::vec3 newVoxelRayPosition = glm::floor(voxelRayPosition / size) * size;

            localRayPosition += voxelRayPosition - newVoxelRayPosition;
            voxelRayPosition = newVoxelRayPosition;

            depth--;

            exitOctree = depth > 0 &&
                glm::abs(
                            glm::dot(
                                 glm::mod(voxelRayPosition / size + 0.5f, 2.0)
                                 - 1.0f + mask * glm::sign(rayDirection) * 0.5f,
                                 mask
                             )
                        ) < 0.1f;
            continue;
        }

        const Node& node = getNode(glm::ivec3(voxelRayPosition), depth);
        traceStack.nodesStack.push_back(node);

        if (node.color.a != -1.0f) {
            traceStack.voxelPos = glm::ivec3(octreePosition + voxelRayPosition);
            break;
        }

        if (node.sub != -1 && depth <= maxDepth) {
            depth++;
            size *= 0.5;

            glm::vec3 mask2 = glm::step(glm::vec3(size), localRayPosition) * size;
            voxelRayPosition += mask2;
            localRayPosition -= mask2;
        } else if (node.sub == -1 || depth > maxDepth) {
            const glm::vec3 hit = insideCubeHit(localRayPosition, rayDirection, rayStepSizeSingle, size);

            mask = glm::vec3(
                glm::lessThan(
                    hit,
                    glm::min(
                        glm::vec3(hit.y, hit.z, hit.x),
                        glm::vec3(hit.z, hit.x, hit.y)
                    )
                )
            );
            const float len = glm::dot(hit, mask);

            distance += len;
            localRayPosition += rayDirection * len - mask * glm::sign(rayDirection) * size;

            glm::vec3 temp = rayStartPosition + distance * rayDirection;
            traceStack.entryStack.emplace_back(temp);

            glm::vec3 newVoxelRayPosition = voxelRayPosition + mask * glm::sign(rayDirection) * size;
            exitOctree = (glm::floor(newVoxelRayPosition / size * 0.5f) != glm::floor(voxelRayPosition / size * 0.5f))
                         && (depth > 0);

            voxelRayPosition = newVoxelRayPosition;
        }
    }

    return traceStack;
}
