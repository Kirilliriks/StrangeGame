//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_NODE_H
#define STRANGEGAME_NODE_H

#include <vector>
#include "glm/glm.hpp"
#include "../utils/Color.h"

struct Node {
    Node(int size, const glm::ivec3& position);

    bool isEmpty();
    int getSubIndex(const glm::ivec3& vec);
    void divide(std::vector<Node> &nodes);
    void setColor(Color color);
    int getSubNodeIndex(const glm::ivec3& vec);
    int getSubNodeIndex(int subIndex);

    glm::ivec3 position;
    int halfSize;
    int sub;
    bool leaf;
    Color color;
};


#endif //STRANGEGAME_NODE_H
