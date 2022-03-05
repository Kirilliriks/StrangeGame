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

struct HitPoint {
    float distance;
    bool hit;
    glm::vec3 color;

    bool debug;
    glm::ivec3 debugPos;

    glm::ivec3 step;
};

glm::ivec3 getSubVector(const glm::ivec3& vec, const Node& node) {
    glm::ivec3 subVector = glm::ivec3(0, 0, 0);

    const glm::ivec3 pos = glm::ivec3(node.position);
    if (vec.x < pos.x) return glm::ivec3(-1, -1, -1);
    if (vec.y < pos.y) return glm::ivec3(-1, -1, -1);
    if (vec.z < pos.z) return glm::ivec3(-1, -1, -1);

    const int halfSize = node.halfSize;
    const int size = halfSize * 2;
    if (vec.x > pos.x + size) return glm::ivec3(-1, -1, -1);
    if (vec.y > pos.y + size) return glm::ivec3(-1, -1, -1);
    if (vec.z > pos.z + size) return glm::ivec3(-1, -1, -1);

    subVector.x = vec.x >= pos.x + halfSize? 1 : 0;
    subVector.y = vec.y >= pos.y + halfSize? 1 : 0;
    subVector.z = vec.z >= pos.z + halfSize? 1 : 0;

    return subVector;
}

int getSubIndex(glm::ivec3 vec, const Node& node) {
    int subIndex = 0;

    const glm::ivec3 pos = glm::ivec3(node.position);
    if (vec.x < pos.x) return -1;
    if (vec.y < pos.y) return -2;
    if (vec.z < pos.z) return -3;

    const int halfSize = node.halfSize;
    const int size = halfSize * 2;
    if (vec.x >= pos.x + size) return -4;
    if (vec.y >= pos.y + size) return -5;
    if (vec.z >= pos.z + size) return -6;

    subIndex |= vec.x >= pos.x + halfSize? 2 : 0;
    subIndex |= vec.y >= pos.y + halfSize? 4 : 0;
    subIndex |= vec.z >= pos.z + halfSize? 1 : 0;

    return subIndex;
}

int getSubIndex(glm::ivec3 vec, const Node& node, const HitPoint& result) {
    int subIndex = 0;

    //vec += glm::ivec3(-1, -1, -1);
    const glm::ivec3 pos = glm::ivec3(node.position);
    if (vec.x < pos.x) return -1;
    if (vec.y < pos.y) return -2;
    if (vec.z < pos.z) return -3;

    const int halfSize = node.halfSize;
    const int size = halfSize * 2;
    if (vec.x >= pos.x + size) return -4;
    if (vec.y >= pos.y + size) return -5;
    if (vec.z >= pos.z + size) return -6;

    subIndex |= vec.x >= pos.x + halfSize? 2 : 0;
    subIndex |= vec.y >= pos.y + halfSize? 4 : 0;
    subIndex |= vec.z >= pos.z + halfSize? 1 : 0;

    return subIndex;
}

void calculate_rayLength(const Node& currentNode, const glm::vec3& start_position, const glm::vec3& rayDirection, const glm::vec3& rayStepSize, glm::vec3 &rayLength, HitPoint &result) {
    const glm::ivec3 node_pos = glm::ivec3(currentNode.position);
    const int size = currentNode.halfSize * 2;
    const glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));

    if (rayDirection.x < 0) {
        rayLength.x += (start_position.x - float(node_pos.x)) * rayStepSize.x;
    } else {
        rayLength.x += (float(node_pos.x + size) - start_position.x) * rayStepSize.x;
    }

    if (rayDirection.y < 0) {
        rayLength.y += (start_position.y - float(node_pos.y)) * rayStepSize.y;
    } else {
        rayLength.y += (float(node_pos.y + size) - start_position.y) * rayStepSize.y;
    }

    if (rayDirection.z < 0) {
        rayLength.z += (start_position.z - float(node_pos.z)) * rayStepSize.z;
    } else {
        rayLength.z += (float(node_pos.z + size) - start_position.z) * rayStepSize.z;
    }

    if (rayLength.x < rayLength.y) {
        if (rayLength.x < rayLength.z) {
            if (step.x < 0) {
                result.step.x = -1;
            }
            result.distance = rayLength.x;
        } else {
            if (step.z < 0) {
                result.step.z = -1;
            }
            result.distance = rayLength.z;
        }
    } else {
        if (rayLength.y < rayLength.z) {
            if (step.y < 0) {
                result.step.y = -1;
            }
            result.distance = rayLength.y;
        } else {
            if (step.z < 0) {
                result.step.z = -1;
            }
            result.distance = rayLength.z;
        }
    }
}

bool move_next(const Layer& layer, const glm::ivec3& step, glm::vec3 &rayLength, HitPoint &result) {
    bool out = true;
    if (rayLength.x < rayLength.y) {
        if (rayLength.x < rayLength.z) {
//            const int tmp = layer.subVector.x + step.x;
//            if (tmp > 1 || tmp < 0) out = false;

            result.distance = rayLength.x;
            rayLength.x += layer.step.x;
            if (layer.step.x < 0) {
                result.step.x--;
            }
        } else {
//            const int tmp = layer.subVector.z + step.z;
//            if (tmp > 1 || tmp < 0) out = false;

            result.distance = rayLength.z;
            rayLength.z += layer.step.z;
            if (layer.step.z < 0) {
                result.step.z--;
            }
        }
    } else {
        if (rayLength.y < rayLength.z) {
//            const int tmp = layer.subVector.y + step.y;
//            if (tmp > 1 || tmp < 0) out = false;

            result.distance = rayLength.y;
            rayLength.y += layer.step.y;
            if (layer.step.y < 0) {
                result.step.y--;
            }
        } else {
//            const int tmp = layer.subVector.z + step.z;
//            if (tmp > 1 || tmp < 0) out = false;

            result.distance = rayLength.z;
            rayLength.z += layer.step.z;
            if (layer.step.z < 0) {
                result.step.z--;
            }
        }
    }
    return out;
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

        const int subIndex = getSubIndex(voxelPos, currentNode);
        if (subIndex < 0) return -1;

        index = currentNode.sub + subIndex;
    }
    return -1;
}

glm::ivec3 Octree::voxelRaycast(const glm::vec3& rayDirection, const glm::vec3& start_position, float maxDistance) {
    HitPoint result;
    result.debug = false;
    result.hit = false;
    result.distance = 0.0f;

    // ВЫЧИСЛЯЕТСЯ РАССТОЯНИЕ ДО СЛЕДУЮЩЕГО ПЕРЕСЕЧЕНИЯ ПО ОСИ ЕСЛИ 0.1 =
    // угол по X то 1 / 0.1 = 10 значит через расстояние = 10 луч пересечётся с осью Y
    const glm::vec3 rayStepSize = glm::abs(glm::vec3(1.0f) / rayDirection);

    glm::vec3 rayLength;
    glm::ivec3 voxelPos = glm::ivec3(start_position);
    glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));

    if (rayDirection.x < 0) {
        rayLength.x = (start_position.x - float(voxelPos.x)) * rayStepSize.x;
    } else {
        rayLength.x = (float(voxelPos.x + 1) - start_position.x) * rayStepSize.x;
    }

    if (rayDirection.y < 0) {
        rayLength.y = (start_position.y - float(voxelPos.y)) * rayStepSize.y;
    } else {
        rayLength.y = (float(voxelPos.y + 1) - start_position.y) * rayStepSize.y;
    }

    if (rayDirection.z < 0) {
        rayLength.z = (start_position.z - float(voxelPos.z)) * rayStepSize.z;
    } else {
        rayLength.z = (float(voxelPos.z + 1) - start_position.z) * rayStepSize.z;
    }

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

Octree::DebugCast Octree::castNode(const glm::vec3& rayDirection, const glm::vec3& start_position) {
    HitPoint result;
    result.debug = false;
    result.hit = false;
    result.distance = 0.0f;

    DebugCast debugCast;
    debugCast.voxelPos = glm::ivec3(-1, -1, -1);
    debugCast.lastStepPos =  glm::ivec3(-1, -1, -1);
    debugCast.nodePos =  glm::ivec3(-1, -1, -1);
    debugCast.nodeSize = 0;
    debugCast.depth = false;
    debugCast.passedNodes = 0;
    debugCast.step = glm::ivec3(0, 0, 0);

    const glm::vec3 rayStep = glm::abs((1.0f / rayDirection));
    const glm::ivec3 step = glm::ivec3(glm::sign(rayDirection));

    glm::vec3 rayLength;
    glm::ivec3 voxelPos = glm::ivec3(start_position);

    Layer layers[maxDepth + 1];

    int currentDepth = maxDepth;

    int index = 0;
    Layer currentLayer;
    Node currentNode = nodes[index];
    currentLayer.step = glm::abs(glm::vec3(currentNode.halfSize * 2) / rayDirection);
    currentLayer.nodeIndex = index;
    layers[currentDepth] = currentLayer;

    bool first = true;
    int iter = -1;

    while (iter++ < 300 && index != -1) {
        debugCast.nodeSize = currentNode.halfSize * 2;
        debugCast.nodePos = glm::ivec3(currentNode.position);

        currentNode = nodes[index];
        currentLayer = layers[currentDepth];

        if (currentNode.sub != -1) { // Node have subNodes
            const int subIndex = getSubIndex(voxelPos, currentNode, result);
            if (subIndex < 0) {
                //debugCast.lastStepPos = glm::ivec3(-1, -1, -1);
                //return debugCast; // EXIT, or maybe check error?
                currentDepth++;
                if (currentDepth > maxDepth) {
                    debugCast.distance = 666;
                    debugCast.depth = true;
                    return debugCast;
                }
                index = layers[currentDepth].nodeIndex;
                continue;
            }

            index = currentNode.sub + subIndex;
            currentDepth--;

            Layer newLayer;
            const Node nextNode = nodes[index];
            newLayer.step = glm::abs(glm::vec3(currentNode.halfSize) / rayDirection); // Use currentNode.halfsize e.t.c subnode have halfsize = halsize / 2;
            newLayer.nodeIndex = index;


            debugCast.passedNodes++;

            layers[currentDepth] = newLayer;
            continue;
        } else { // Node can be voxel or be empty
            if (currentNode.color.a != -1.0f) { //Node is voxel
                debugCast.voxelPos = glm::ivec3(currentNode.position);
                debugCast.distance = result.distance;
                return debugCast;
            } else { // Find next node
                if (first) {
                    first = false;
                    calculate_rayLength(currentNode, start_position, rayDirection, rayStep, rayLength, result);
                } else {
                    move_next(currentLayer, step, rayLength, result);
                }
                debugCast.preLastStepPos = voxelPos;
                debugCast.lastStepPos = voxelPos = start_position + rayDirection * (result.distance + 0.000001f); // Calculate current voxelPosition
                debugCast.voxelFloatPos = voxelPos;
                debugCast.step = result.step;

                currentDepth++;
                if (currentDepth > maxDepth) {
                    debugCast.depth = true;
                    debugCast.distance = 333;
                    return debugCast;
                }
                index = layers[currentDepth].nodeIndex;
            }
        }
    }

    return debugCast;
}


