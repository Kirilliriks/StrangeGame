//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_NODE_H
#define STRANGEGAME_NODE_H

#include <vector>
#include "glm/glm.hpp"
#include "../utils/Color.h"

struct Node {
    Node(int size, glm::ivec3 position);

    bool isEmpty();
    int getSubIndex(const glm::ivec3& vec);
    void divide(std::vector<Node> &nodes);
    void setVoxel(glm::vec4 color);
    int getSubNodeIndex(const glm::ivec3& vec);
    int getSubNodeIndex(int subIndex);

    alignas(16) glm::ivec4 position;
    alignas(16) glm::vec4 color;
    int halfSize;
    int sub;
};


#endif //STRANGEGAME_NODE_H
