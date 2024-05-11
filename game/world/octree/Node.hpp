//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_NODE_H
#define STRANGEGAME_NODE_H

#include "glm/glm.hpp"

struct Node {
    Node(const glm::ivec3& position);
    Node(const glm::vec4& color);

    bool isEmpty() const;
    int getSubIndex(const int& halfSize, const glm::ivec3& vec) const;
    void setVoxel(const glm::vec4& color);
    int getSubNodeIndex(const int& halfSize, const glm::ivec3& vec) const;
    int getSubNodeIndex(const int& subIndex) const;

    alignas(16) glm::ivec4 position; // TODO remove? Если мы знаем изначальный центр дерева то по идее можем вычислить координаты вокселя
    alignas(16) glm::vec4 color;
    int sub;
};


#endif //STRANGEGAME_NODE_H
