//
// Created by KirilliriK on 17.02.2024.
//

#include "Mesh.hpp"
#include <glad.h>
#include <iostream>

#include "MeshBuilder.hpp"

float vertic[] = {
    // x    y     z
    -1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

     1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};

Mesh::Mesh(MeshBuilder& meshBuilder)
: Mesh(meshBuilder.getBuffer(), meshBuilder.getVertexes(), meshBuilder.getAttributes()) {

}

Mesh::Mesh(const std::vector<float>& buffer, const int& vertexes, const std::vector<int>& attributes) {
    int vertexSize = 0;
    for (const int attribute : attributes) {
        vertexSize += attribute;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertic), &vertic, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertexes, &buffer[0], GL_STATIC_DRAW);

    // attributes
    int offset = 0;
    int i = 0;
    for (const int attribute : attributes) {
        glVertexAttribPointer(i, attribute, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += attribute;
        i++;
    }

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::render() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
