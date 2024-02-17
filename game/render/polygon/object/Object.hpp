//
// Created by KirilliriK on 17.02.2024.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "glm/glm.hpp"
#include "../mesh/Mesh.hpp"

class Object {
public:
    explicit Object(const glm::vec3& position);

    void render() const;
    const glm::mat4& getModelMatrix() const;
private:
    Mesh mesh;

    glm::vec3 position;
    glm::mat4 model;
};



#endif //OBJECT_HPP
