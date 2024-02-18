//
// Created by KirilliriK on 17.02.2024.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "glm/glm.hpp"
#include "../mesh/Mesh.hpp"

class Object {
public:
    Object() = default;
    Object(const glm::vec3& position, Mesh* mesh);

    void render() const;
    const glm::mat4& getModelMatrix() const;
    void setToRemove();
    bool isNeedRemove() const;
private:
    Mesh* mesh = nullptr;

    glm::vec3 position;
    glm::mat4 model;

    bool toRemove = false;
};



#endif //OBJECT_HPP
