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
        glm::ivec3 lastStepPos;
        glm::ivec3 preLastStepPos;
        glm::ivec3 nodePos;
        int nodeSize;
        float distance;

        bool depth;

        glm::vec3 voxelFloatPos;
        int passedNodes;

        glm::ivec3 step;

        glm::ivec3 subVector;
    };

    struct HitPoint {
        float distance;
        glm::ivec3 step;
        glm::ivec3 dir;
        glm::vec3 rayDirection;
        glm::vec3 rayStepSize;
        DebugCast debugCast;

        int iter;

        bool found;
    };

    Octree();
    int getSize();
    void setVoxel(const glm::ivec3& vec, glm::vec4 color);

    Node *getData();
    int nodesCount();

    DebugCast raycastVoxel(const glm::vec3& rayDirection, const glm::vec3& start_position);
private:
    void setVoxel(int index, int depth, const glm::ivec3& vec, glm::vec4 color);

    std::vector<Node> nodes;
    int maxDepth;
    int size;
};


#endif //STRANGEGAME_OCTREE_H
