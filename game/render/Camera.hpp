//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_CAMERA_H
#define STRANGEGAME_CAMERA_H

#define GLFW_INCLUDE_NONE
#include "glm/glm.hpp"
#include "Window.hpp"

class Camera {
public:
    Camera(Window *window);

    void update(double deltaTime, float mouseX, float mouseY);
    float getYaw();
    float getPitch();
    float getX();
    float getY();
    float getZ();

    glm::vec3 getPosition();
    glm::vec3 getDirection();
private:
    Window *window;

    glm::vec3 direction;
    glm::vec3 position;
    float yaw;
    float pitch;

    float speed = 60;
};


#endif //STRANGEGAME_CAMERA_H
