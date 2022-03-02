//
// Created by KirilliriK on 28.02.2022.
//

#include "Camera.h"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include <string>

Camera::Camera(Window *window) {
    this->window = window;

    direction = glm::vec3(0, 0, -1);
    yaw = 7.0f;
    pitch = 0.0f;
    position = glm::vec3(-1.5, 3, -1.5);
}

void Camera::update(double deltaTime, float mouseX, float mouseY) {
    std::string title;
    title.append("X ");
    title.append(std::to_string(position.x));
    title.append(" y ");
    title.append(std::to_string(position.y));
    title.append(" Z ");
    title.append(std::to_string(position.z));
    title.append(" YAW ");
    title.append(std::to_string(glm::degrees(yaw)));
    title.append(" PITCH ");
    title.append(std::to_string(glm::degrees(pitch)));
    glfwSetWindowTitle(window->getGLWindow(), title.c_str());
    glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 movementVector = direction;

    // Keys management
    GLFWwindow * glWindow = window->getGLWindow();
    int state = glfwGetKey(glWindow, GLFW_KEY_W);
    if (state == GLFW_PRESS) {
        movement = -speed * movementVector;
    }

    state = glfwGetKey(glWindow, GLFW_KEY_S);
    if (state == GLFW_PRESS) {
        movement = speed * movementVector;
    }

    state = glfwGetKey(glWindow, GLFW_KEY_A);
    if (state == GLFW_PRESS) {
        movement = speed * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
    }

    state = glfwGetKey(glWindow, GLFW_KEY_D);
    if (state == GLFW_PRESS) {
        movement = speed * glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    state = glfwGetKey(glWindow, GLFW_KEY_SPACE);
    if (state == GLFW_PRESS) {
        position.y += speed * deltaTime;
    }

    state = glfwGetKey(glWindow, GLFW_KEY_ESCAPE);
    if (state == GLFW_PRESS) {
        exit(0);
    }

    position += glm::vec3(movement.x * deltaTime, movement.y * deltaTime, movement.z * deltaTime);

    yaw = (mouseX - (float)window->width / 2.0f) * 0.005f;
    float lastPitch = pitch;
    pitch = (mouseY - (float)window->height / 2.0f) * 0.005f;
    if (glm::degrees(pitch) > 90 || glm::degrees(pitch) < -90) pitch = lastPitch;

    direction = getDirection();
}

float Camera::getYaw() {
    return yaw;
}

float Camera::getPitch() {
    return pitch;
}

float Camera::getX() {
    return position.x;
}

float Camera::getY() {
    return position.y;
}

float Camera::getZ() {
    return position.z;
}

glm::vec3 Camera::getDirection() {
    const float sinAng = std::sin(-yaw);
    const float cosAng = std::cos(-yaw);
    const float sinAngY = std::sin(-pitch);
    const float cosAngY = std::cos(-pitch);

    glm::vec3 dir(0,
                  -1 * sinAngY,
                  -1 * cosAngY);

    return glm::vec3(dir.x * cosAng - dir.z * sinAng,
                     dir.y,
                     dir.x * sinAng + dir.z * cosAng);
}
