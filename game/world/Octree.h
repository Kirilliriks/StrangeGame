//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_OCTREE_H
#define STRANGEGAME_OCTREE_H

#include <vector>
#include "Node.h"

class Octree {
public:
    struct VoxelInfo {
        glm::ivec3 voxelPos;
        glm::ivec3 nodePos;
        int nodeSize;
        float distance;

        glm::ivec3 step;
        glm::ivec3 subVector;
    };

    Octree();
    int getSize();
    void setVoxel(const glm::ivec3& vec, glm::vec4 color);

    Node *getData();
    glm::vec4 *getVData();
    int nodesCount();

    VoxelInfo raycastVoxel(const glm::vec3& rayDirection, const glm::vec3& start_position);

    int height;
private:
    void setVoxel(int index, int depth, const glm::ivec3& vec, const glm::vec4& color);

    std::vector<Node> nodes;
    std::vector<glm::vec4> voxels;
    int maxDepth;
    int size;
};


#endif //STRANGEGAME_OCTREE_H
