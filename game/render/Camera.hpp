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
    explicit Camera(Window *window);

    void update(const double& deltaTime, const float& mouseX, const float& mouseY);
    float getYaw() const;
    float getPitch() const;
    float getX() const;
    float getY() const;
    float getZ() const;

    void updateVectors();
    void updateMatrix();

    glm::vec3 getPosition();
    glm::vec3 getDirection() const;

    glm::mat4 getProjection() const;
    glm::mat4 getView() const;
    glm::mat4 getMatrixMultiply() const;
private:
    Window *window;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::mat4 rotation;

    glm::vec3 direction;
    glm::vec3 position;
    float yaw;
    float pitch;
    float fov;

    float speed;
};


#endif //STRANGEGAME_CAMERA_H
