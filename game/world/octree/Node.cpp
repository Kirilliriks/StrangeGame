//
// Created by KirilliriK on 02.03.2022.
//
#include "Node.hpp"

// Little help struct, TODO delete
//enum nodeIndex {
//    DOWN_LEFT_FRONT = 0, //000
//    DOWN_LEFT_BACK = 1,  //001
//    DOWN_RIGHT_FRONT = 2,//010
//    DOWN_RIGHT_BACK = 3, //011
//    UPPER_LEFT_FRONT = 4,  //100
//    UPPER_LEFT_BACK = 5,   //101
//    UPPER_RIGHT_FRONT = 6, //110
//    UPPER_RIGHT_BACK = 7   //111
//};

Node::Node(const glm::ivec3& position) {
    this->position = glm::vec4(position, 0.0f);
    sub = -1;
    color = glm::vec4(0.0f, 0.0f, 0.0f, -1.0f);
}

bool Node::isEmpty() const {
    return sub == -1;
}

void Node::setVoxel(const glm::vec4& color) {
    this->color = color;
}

int Node::getSubIndex(const int& halfSize, const glm::ivec3& vec) const {
    if (vec.x < position.x || vec.y < position.y || vec.z < position.z) return -1;

    if (const int size = halfSize * 2;
        vec.x > position.x + size ||
        vec.y > position.y + size ||
        vec.z > position.z + size
    ) {
        return -1;
    }

    int subIndex = 0;
    subIndex |= vec.x >= (position.x + halfSize) ? 2 : 0;
    subIndex |= vec.y >= (position.y + halfSize) ? 4 : 0;
    subIndex |= vec.z >= (position.z + halfSize) ? 1 : 0;

    return subIndex;
}

int Node::getSubNodeIndex(const int& halfSize, const glm::ivec3& vec) const {
    return getSubNodeIndex(getSubIndex(halfSize, vec));
}

int Node::getSubNodeIndex(const int& subIndex) const {
    return sub + subIndex;
}