//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_NODE_H
#define STRANGEGAME_NODE_H

#include <vector>
#include "glm/glm.hpp"

struct Node {
    Node(int size, const glm::ivec3& position);

    bool isEmpty() const;
    int getSubIndex(const glm::ivec3& vec) const;
    void divide(std::vector<Node> &nodes);
    void setVoxel(const glm::vec4& color);
    int getSubNodeIndex(const glm::ivec3& vec) const;
    int getSubNodeIndex(int subIndex) const;

    alignas(16) glm::ivec4 position; // TODO remove? Если мы знаем изначальный центр дерева то по идее можем вычислить координаты вокселя
    alignas(16) glm::vec4 color;
    int halfSize; // TODO remove? Если мы знаем изначальный размер дерева то по идее можем вычислить размер ноды
    int sub;
};


#endif //STRANGEGAME_NODE_H
