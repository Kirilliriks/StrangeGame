//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_NODE_H
#define STRANGEGAME_NODE_H

#include <vector>
#include "../utils/Color.h"
#include "glm/glm.hpp"

class Node {
public:
    Node(int index, int size, glm::ivec3 position);

    int getSubIndex(glm::ivec3 vec);
    bool isEmpty();
    void divide(std::vector<Node> &nodes);
    void setColor(Color color);
    int getIndex();
    glm::ivec3 getPosition();
    int getSubNodeIndex(glm::ivec3 vec);
    int getSubNodeIndex(int subIndex);
private:
    glm::ivec3 position;
    int size; // TODO maybe remove, redundant for shader
    int index;
    int subs[8]; // TODO optimize, можно использовать лишь subIndex для 0-ой подноды, а остальные это subIndex + n
    bool leaf; // if true it is voxel
    Color color;
};


#endif //STRANGEGAME_NODE_H
