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

Node::Node(const int& size, const glm::ivec3& position) {
    this->halfSize = size / 2;
    this->position = glm::vec4(position, 0.0f);
    sub = -1;
    color = glm::vec4(0.0f, 0.0f, 0.0f, -1.0f);
}

bool Node::isEmpty() const {
    return sub == -1;
}

void Node::divide(std::vector<Node>& nodes) {
    sub = nodes.size();
    for (int i = 0; i < 8; i++) {
        auto pos = glm::ivec3(position);

        if ((i & 2) == 2) {
            pos.x += halfSize;
        }

        if ((i & 4) == 4) {
            pos.y += halfSize;
        }

        if ((i & 1) == 1) {
            pos.z += halfSize;
        }

        nodes.emplace_back(halfSize, pos);
    }
}

int Node::getSubIndex(const glm::ivec3& vec) const {
    int subIndex = 0;
    subIndex |= vec.x >= (position.x + halfSize) ? 2 : 0;
    subIndex |= vec.y >= (position.y + halfSize) ? 4 : 0;
    subIndex |= vec.z >= (position.z + halfSize) ? 1 : 0;

    return subIndex;
}

void Node::setVoxel(const glm::vec4& color) {
    this->color = color;
}

int Node::getSubNodeIndex(const glm::ivec3& vec) const {
    return getSubNodeIndex(getSubIndex(vec));
}

int Node::getSubNodeIndex(const int& subIndex) const {
    return sub + subIndex;
}