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

struct Layer {
    int nodeIndex;
    glm::vec3 step;
    glm::ivec3 subVector;
};

glm::ivec3 getSubVector(const glm::vec3& vec, const Node& node) {
    glm::ivec3 subVector;

    const glm::vec3 pos = glm::vec3(node.position);
    if (vec.x < pos.x) return glm::ivec3(-1);
    if (vec.y < pos.y) return glm::ivec3(-1);
    if (vec.z < pos.z) return glm::ivec3(-1);

    const float halfSize = (float)node.halfSize;
    const float size = halfSize * 2;
    if (vec.x > pos.x + size) return glm::ivec3(-1);
    if (vec.y > pos.y + size) return glm::ivec3(-1);
    if (vec.z > pos.z + size) return glm::ivec3(-1);

    subVector.x = vec.x >= pos.x + halfSize? 1 : 0;
    subVector.y = vec.y >= pos.y + halfSize? 1 : 0;
    subVector.z = vec.z >= pos.z + halfSize? 1 : 0;

    return subVector;
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

//int getSubIndex(glm::ivec3 vec, const Node& node, const HitPoint& result) {
//    int subIndex = 0;
//
//    vec += glm::ivec3(-1, -1, -1);
//    const glm::ivec3 pos = glm::ivec3(node.position);
//    if (vec.x < pos.x) return -1;
//    if (vec.y < pos.y) return -2;
//    if (vec.z < pos.z) return -3;
//
//    const int halfSize = node.halfSize;
//    const int size = halfSize * 2;
//    if (vec.x >= pos.x + size) return -4;
//    if (vec.y >= pos.y + size) return -5;
//    if (vec.z >= pos.z + size) return -6;
//
//    subIndex |= vec.x >= pos.x + halfSize? 2 : 0;
//    subIndex |= vec.y >= pos.y + halfSize? 4 : 0;
//    subIndex |= vec.z >= pos.z + halfSize? 1 : 0;
//
//    return subIndex;
//}
//
//void calculate_rayLength(const Node& currentNode, const glm::vec3& start_position, const glm::vec3& rayDirection, const glm::vec3& rayStepSize, glm::vec3 &rayLength, HitPoint &result) {
//    const glm::ivec3 node_pos = glm::ivec3(currentNode.position);
//    const int size = currentNode.halfSize * 2;
//    const glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));
//
//    if (rayDirection.x < 0) {
//        rayLength.x += (start_position.x - float(node_pos.x)) * rayStepSize.x;
//    } else {
//        rayLength.x += (float(node_pos.x + size) - start_position.x) * rayStepSize.x;
//    }
//
//    if (rayDirection.y < 0) {
//        rayLength.y += (start_position.y - float(node_pos.y)) * rayStepSize.y;
//    } else {
//        rayLength.y += (float(node_pos.y + size) - start_position.y) * rayStepSize.y;
//    }
//
//    if (rayDirection.z < 0) {
//        rayLength.z += (start_position.z - float(node_pos.z)) * rayStepSize.z;
//    } else {
//        rayLength.z += (float(node_pos.z + size) - start_position.z) * rayStepSize.z;
//    }
//
//    if (rayLength.x < rayLength.y) {
//        if (rayLength.x < rayLength.z) {
//            if (step.x < 0) {
//                result.step.x = -1;
//            }
//            result.distance = rayLength.x;
//        } else {
//            if (step.z < 0) {
//                result.step.z = -1;
//            }
//            result.distance = rayLength.z;
//        }
//    } else {
//        if (rayLength.y < rayLength.z) {
//            if (step.y < 0) {
//                result.step.y = -1;
//            }
//            result.distance = rayLength.y;
//        } else {
//            if (step.z < 0) {
//                result.step.z = -1;
//            }
//            result.distance = rayLength.z;
//        }
//    }
//}
//
//bool move_next(const Layer& layer, const glm::ivec3& step, glm::vec3 &rayLength, HitPoint &result) {
//    bool out = true;
//    if (rayLength.x < rayLength.y) {
//        if (rayLength.x < rayLength.z) {
//            if (step.x < 0) {
//                result.step.x = -1;
//            }
//            result.distance = rayLength.x;
//            rayLength.x += layer.step.x;
//        } else {
//            if (step.z < 0) {
//                result.step.z = -1;
//            }
//            result.distance = rayLength.z;
//            rayLength.z += layer.step.z;
//        }
//    } else {
//        if (rayLength.y < rayLength.z) {
//            if (step.y < 0) {
//                result.step.y = -1;
//            }
//            result.distance = rayLength.y;
//            rayLength.y += layer.step.y;
//        } else {
//            if (step.z < 0) {
//                result.step.z = -1;
//            }
//            result.distance = rayLength.z;
//            rayLength.z += layer.step.z;
//        }
//    }
//    return out;
//}

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

glm::ivec3 Octree::voxelRaycast(const glm::vec3& rayDirection, const glm::vec3& start_position, float maxDistance) {
    HitPoint result;
    result.distance = 0.0f;

    const glm::vec3 rayStepSize = glm::abs(glm::vec3(1.0f) / rayDirection);

    glm::vec3 rayLength;
    glm::ivec3 voxelPos = glm::ivec3(start_position);
    glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));

    glm::ivec3 dir = glm::ivec3(rayDirection.x > 0.0f, rayDirection.y > 0.0f, rayDirection.z > 0.0f);
    rayLength = (glm::vec3(glm::ivec3(start_position) + dir) - start_position) / rayDirection;

    while(result.distance < maxDistance) {
        if (rayLength.x < rayLength.y) {
            if (rayLength.x < rayLength.z) {
                voxelPos.x += step.x;
                result.distance = rayLength.x;
                rayLength.x += rayStepSize.x;
            } else {
                voxelPos.z += step.z;
                result.distance = rayLength.z;
                rayLength.z += rayStepSize.z;
            }
        } else {
            if (rayLength.y < rayLength.z) {
                voxelPos.y += step.y;
                result.distance = rayLength.y;
                rayLength.y += rayStepSize.y;
            } else {
                voxelPos.z += step.z;
                result.distance = rayLength.z;
                rayLength.z += rayStepSize.z;
            }
        }

        if (findVoxel(voxelPos) == 1) {
            return voxelPos;
        }
    }
    return glm::ivec3(-1, -1, -1);
}

void Octree::recurseCast(HitPoint &result, const glm::vec3& position, const int &halfSize, const Node &currentNode) {
    if (halfSize == 0) return;

    glm::ivec3 subVec = glm::min(glm::ivec3(position) / halfSize, 1);
    glm::vec3 rayLength = (glm::vec3(subVec + result.dir) * float(halfSize) - position) * result.rayStepSize;

    const glm::vec3 rayStepSize = glm::vec3(halfSize) * glm::abs(result.rayStepSize);

    float distance = 0.0f;
    const float lastDistance = result.distance;
    while (result.iter < 500) {
        const int subIndex = getSubIndexFromSubVector(subVec);
        if (subIndex < 0) return;
        result.iter++;

        if (currentNode.sub != -1) {
            const Node subNode = nodes[currentNode.sub + subIndex];

            if (subNode.color.a != -1.0f) {
                result.debugCast.voxelPos = glm::ivec3(subNode.position);
                result.found = true;
                return;
            } else {
                const int subSize = halfSize / 2;
                const glm::vec3 subPosition = position + distance * result.rayDirection - glm::vec3(halfSize * subVec);
                result.distance = lastDistance + distance;
                result.debugCast.subVector = subVec;

                result.debugCast.nodeSize = halfSize;
                result.debugCast.nodePos = glm::ivec3(subNode.position);

                recurseCast(result, subPosition, subSize, subNode);
                if (result.found) {
                    return;
                }
            }
        }

        if (rayLength.x < rayLength.y) {
            if (rayLength.x < rayLength.z) {
                distance = rayLength.x;
                rayLength.x += rayStepSize.x;
                subVec.x += result.step.x;
            } else {
                distance = rayLength.z;
                rayLength.z += rayStepSize.z;
                subVec.z += result.step.z;
            }
        } else {
            if (rayLength.y < rayLength.z) {
                distance = rayLength.y;
                rayLength.y += rayStepSize.y;
                subVec.y += result.step.y;
            } else {
                distance = rayLength.z;
                rayLength.z += rayStepSize.z;
                subVec.z += result.step.z;
            }
        }
    }
}

Octree::DebugCast Octree::castNode(const glm::vec3& rayDirection, const glm::vec3& start_position) {
    HitPoint result;
    result.rayDirection = rayDirection;
    result.rayStepSize = 1.0f / rayDirection;
    result.step = glm::ivec3(glm::sign(rayDirection));
    result.dir = glm::max(result.step, 0);
    result.iter = 0;
    result.distance = 0.0f;
    result.debugCast.test = true;
    result.found = false;
    recurseCast(result, start_position, nodes[0].halfSize, nodes[0]);
    return result.debugCast;
}
