//
// Created by KirilliriK on 24.02.2024.
//

#ifndef TRACESTACK_HPP
#define TRACESTACK_HPP

#include <vector>
#include <glm/glm.hpp>

#include "Node.hpp"

class TraceStack {
public:
    std::vector<glm::vec3> entryStack;
    std::vector<Node> nodesStack;

    glm::ivec3 voxelPos;
    glm::ivec3 preVoxelPos;
    int iterations;
    int iterationsF;
    float distance;
};



#endif //TRACESTACK_HPP
