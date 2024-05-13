//
// Created by KirilliriK on 24.02.2024.
//

#ifndef TRACESTACK_HPP
#define TRACESTACK_HPP

#include <vector>

#include "Node.hpp"

struct TraceStack {
    std::vector<glm::vec3> entryStack;
    std::vector<Node> nodesStack;
    std::vector<Node> pathNodesStack;

    glm::ivec3 voxelPos;
    glm::ivec3 previousVoxelPos;
    int iterations;
    int iterationsF;
    float distance;
};


#endif //TRACESTACK_HPP
