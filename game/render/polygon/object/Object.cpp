//
// Created by KirilliriK on 17.02.2024.
//

#include "Object.hpp"


Object::Object(const glm::vec3& position, Mesh* mesh) : mesh(mesh), position(position) {
    model = glm::mat4(1);
}

void Object::render() const {
    mesh->render();
}

const glm::mat4& Object::getModelMatrix() const {
    return model;
}
