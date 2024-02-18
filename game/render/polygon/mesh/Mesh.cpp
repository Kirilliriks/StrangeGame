//
// Created by KirilliriK on 17.02.2024.
//

#include "Mesh.hpp"
#include <gl.h>
#include <iostream>

#include "MeshBuilder.hpp"

// std::vector vector {
//     -1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//    1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//   -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//
//    1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//   -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
// };

Mesh::Mesh(MeshBuilder& meshBuilder)
: Mesh(meshBuilder.getBuffer(), meshBuilder.getVertexes(), meshBuilder.getAttributes()) {
}

Mesh::Mesh(const std::vector<float>& buffer, const int& vertices, const std::vector<int>& attributes) {
    this->vertices = vertices;

    int vertexSize = 0;
    for (const int attribute : attributes) {
        vertexSize += attribute;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), &buffer.front(), GL_STATIC_DRAW);

    // attributes
    int offset = 0;
    int i = 0;
    for (const int attribute : attributes) {
        glVertexAttribPointer(i, attribute, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += attribute;
        i++;
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::render() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices);
    glBindVertexArray(0);
}

const unsigned int& Mesh::getVertices() const {
    return vertices;
}
