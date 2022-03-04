//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_OCTREE_H
#define STRANGEGAME_OCTREE_H

#include <vector>
#include "Node.h"

class Octree {
public:
    Octree();
    int getSize();
    void setVoxel(const glm::ivec3& vec, glm::vec4 color);

    Node *getData();
    int nodesCount();

    glm::ivec3 castNode(const glm::vec3& rayDirection, const glm::vec3& start_position);
    glm::ivec3 debugCast(const glm::vec3& rayDirection, const glm::vec3& start_position, float maxDistance);
    Node getNode(const glm::ivec3& vec);
private:
    void setVoxel(int index, int depth, const glm::ivec3& vec, glm::vec4 color);

    std::vector<Node> nodes;
    int maxDepth; //
    int size; // in voxels
    int findVoxel(const glm::ivec3 &voxelPos);
};


#endif //STRANGEGAME_OCTREE_H
