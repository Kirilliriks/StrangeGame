//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_OCTREE_H
#define STRANGEGAME_OCTREE_H

#include <vector>
#include "Node.h"

class Octree {
public:
    struct DebugCast {
        glm::ivec3 voxelPos;
        int iterations;
        int iterationsF;
    };

    struct HitPoint {
        float distance;
    };

    Octree();
    int getSize();
    void setVoxel(const glm::ivec3& vec, glm::vec4 color);

    Node *getData();
    int nodesCount();

    DebugCast castDRay(const glm::vec3& rayDirection, const glm::vec3& start_position);
    DebugCast raycastVoxel(const glm::vec3& rayDirection, const glm::vec3& start_position);
    glm::ivec4 voxelRaycast(const glm::vec3 &rayDirection, const glm::vec3 &start_position, float maxDistance);
private:
    void setVoxel(int index, int depth, const glm::ivec3& vec, glm::vec4 color);
    int findVoxel(const glm::ivec3 &voxelPos);

    std::vector<Node> nodes;
    int maxDepth;
    int size;
};


#endif //STRANGEGAME_OCTREE_H
