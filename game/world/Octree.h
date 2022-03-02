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
    void setVoxel(const glm::ivec3&, const Color &color);
private:
    void setVoxel(int index, int depth, const glm::ivec3&, const Color &color);

    std::vector<Node> nodes;
    const int maxDepth = 10; // 4^10 == 1 048 576 voxels MAX
};


#endif //STRANGEGAME_OCTREE_H
