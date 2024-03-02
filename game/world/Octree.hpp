//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_OCTREE_H
#define STRANGEGAME_OCTREE_H

#include <vector>
#include "Node.hpp"
#include "TraceStack.hpp"

class Octree {
public:
    Octree();
    int getSize() const;
    void setVoxel(const glm::ivec3& vec, const glm::vec4& color);
    Node getVoxel(const glm::ivec3& vec);
    Node getNode(const glm::ivec3& vec, int depth) const;
    void removeVoxel(const glm::ivec3& vec);

    Node *getData();
    int nodesCount() const;

    TraceStack voxelRaycastDDA(const glm::vec3 &rayDirection, const glm::vec3 &start_position, float maxDistance) const;
    TraceStack voxelRaycastTraversal(const glm::vec3& rayDirection, const glm::vec3& rayStartPosition) const;
private:
    void setVoxel(const int& index, const int& depth, const glm::ivec3& vec, const glm::vec4& color);
    Node getVoxel(const int& index, const int& depth, const glm::ivec3& vec);
    Node getNode(const int& index, const int& depth, const int& nodeDepth, const glm::ivec3& vec) const;
    void removeVoxel(const int& index, const int& depth, const glm::ivec3& vec);
    int findVoxel(const glm::ivec3 &voxelPos) const;

    std::vector<Node> nodes;
    int maxDepth;
    int size;
};


#endif //STRANGEGAME_OCTREE_H
