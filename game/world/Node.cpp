//
// Created by KirilliriK on 01.03.2022.
//

#include "Node.h"

#include <iostream>

// Little help struct, TODO delete
enum nodeIndex {
    DOWN_LEFT_FRONT = 0, //000
    DOWN_LEFT_BACK = 1,  //001
    DOWN_RIGHT_FRONT = 2,//010
    DOWN_RIGHT_BACK = 3, //011
    UPPER_LEFT_FRONT = 4,  //100
    UPPER_LEFT_BACK = 5,   //101
    UPPER_RIGHT_FRONT = 6, //110
    UPPER_RIGHT_BACK = 7   //111
};


Node::Node(int index, int size, glm::ivec3 position) {
    this->position = position;
    this->index = index;
    this->size = size;
    for (int &sub : subs) {
        sub = -1;
    }
    leaf = false;
    color = Color(0, 0, 0);
}

int Node::getSubIndex(glm::ivec3 vec) {
    int subIndex = 0;

    subIndex |= vec.x > position.x ? 2 : 0;
    subIndex |= vec.y > position.y ? 4 : 0;
    subIndex |= vec.z > position.z ? 1 : 0;

//    std::cout << "! " << subIndex << std::endl;
//    std::cout << vec.x << "=" << position.x << std::endl;
//    std::cout << vec.y << "=" << position.y << std::endl;
//    std::cout << vec.z << "=" << position.z << std::endl;
//    std::cout << "? " << subIndex << std::endl;
    return subIndex;
}

bool Node::isEmpty() {
    return subs[0] == -1;
}

void Node::divide(std::vector<Node> &nodes) {
    for (int i = 0; i < 8; i++) {
        glm::ivec3 pos = position;
        const int offset = (int)((float)size * 0.25f);

        if ((i & 2) == 2) {
            pos.x += offset;
        } else {
            pos.x -= offset;
        }

        if ((i & 4) == 4) {
            pos.y += offset;
        } else {
            pos.y -= offset;
        }

        if ((i & 1) == 1) {
            pos.z += offset;
        } else {
            pos.z -= offset;
        }

//        std::cout << "Index=" << index << std::endl;
//        std::cout << "Offset=" << offset << std::endl;
//        std::cout << pos.x << "=" << position.x << std::endl;
//        std::cout << pos.y << "=" << position.y << std::endl;
//        std::cout << pos.z << "=" << position.z << std::endl;
//        std::cout << "@@@" << std::endl;

        const int sizeNodes = nodes.size();
        subs[i] = sizeNodes;
        nodes.emplace_back(subs[i], size / 2, pos);
    }
}

void Node::setColor(Color color) {
    leaf = true;
    this->color = color;
}

int Node::getIndex() {
    return index;
}

glm::ivec3 Node::getPosition() {
    return position;
}

int Node::getSubNodeIndex(glm::ivec3 vec) {
    return getSubNodeIndex(getSubIndex(vec));
}

int Node::getSubNodeIndex(int subIndex) {
    return subs[subIndex];
}
