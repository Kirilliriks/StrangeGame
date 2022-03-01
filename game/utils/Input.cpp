//
// Created by KirilliriK on 01.03.2022.
//

#include "glm/glm.hpp"

class Input {
public:
    Input() {
        mousePos = glm::vec3(0, 0, 0);
        lastMousePos = glm::vec3(0, 0, 0);
    }

    static void update() {

    }

    static glm::vec3 mousePos;
    static glm::vec3 lastMousePos;
};