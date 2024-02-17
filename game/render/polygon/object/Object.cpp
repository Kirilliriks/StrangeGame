//
// Created by KirilliriK on 17.02.2024.
//

#include "Object.hpp"

#include "../mesh/MeshBuilder.hpp"

Object::Object(const glm::vec3& position) : position(position) {
    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(5, 5, 5), glm::vec3(1, 0, 0), 5);
    mesh = Mesh(meshBuilder);
    model = glm::mat4(1);
}

void Object::render() const {
    mesh.render();
}

const glm::mat4& Object::getModelMatrix() const {
    return model;
}
