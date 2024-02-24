//
// Created by KirilliriK on 28.02.2022.
//

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "../utils/Input.hpp"
#include "../Game.hpp"
#include "imgui.h"

Camera::Camera(Window *window) {
    this->window = window;

    direction = glm::vec3(0, 0, 1);
    yaw = 0;
    pitch = 0;
    position = glm::vec3(0, 0, 1);
    fov = 75;
    speed = 15;

    rotation = glm::mat4(1.0f);
    updateVectors();
}

void Camera::update(const double& deltaTime) {
    GLFWwindow *glWindow = window->getGLWindow();

    double mouseX, mouseY;
    glfwGetCursorPos(glWindow, &mouseX, &mouseY);

    const double windowHalfWidth = window->getHalfWidth();
    const double windowHalfHeight = window->getHalfHeight();
    if (Game::focused) {
        glfwSetCursorPos(glWindow,windowHalfWidth,windowHalfHeight);
    }

    auto movement = glm::vec3(0.0f);

    if (Game::focused) {
        int state = glfwGetKey(glWindow, GLFW_KEY_W);
        if (state == GLFW_PRESS) {
            movement = speed * direction;
        }

        state = glfwGetKey(glWindow, GLFW_KEY_S);
        if (state == GLFW_PRESS) {
            movement = -speed * direction;
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

        state = glfwGetKey(glWindow, GLFW_KEY_LEFT_SHIFT);
        if (state == GLFW_PRESS) {
            position.y -= speed * deltaTime;
        }

        state = glfwGetKey(glWindow, GLFW_KEY_ESCAPE);
        if (state == GLFW_PRESS) {
            exit(0);
        }

        position += glm::vec3(movement.x * deltaTime, movement.y * deltaTime, movement.z * deltaTime);

        yaw -= (mouseX - windowHalfWidth) * 0.005;
        pitch -= (mouseY - windowHalfHeight) * 0.005;

        direction = getDirection();

        updateMatrix();
    }

    if (Input::tab.pressed) {
        Game::focused = !Game::focused;

        ImGuiIO& io = ImGui::GetIO();
        if (Game::focused) {
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;            // Disable Mouse
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard; // Disable Keyboard
            glfwSetCursorPos(glWindow,windowHalfWidth,windowHalfHeight);
            glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;            // Enable Mouse
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard
            glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

float Camera::getYaw() const {
    return yaw;
}

float Camera::getPitch() const {
    return pitch;
}

float Camera::getX() const {
    return position.x;
}

float Camera::getY() const {
    return position.y;
}

float Camera::getZ() const {
    return position.z;
}

void Camera::updateVectors() {
    front = glm::vec3(rotation * glm::vec4(0, 0, -1, 1));
    right = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
    up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));
}

void Camera::updateMatrix() {
    rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, 0.0f, glm::vec3(0, 0, 1));
    rotation = glm::rotate(rotation, glm::degrees(yaw), glm::vec3(0, 1, 0));
    rotation = glm::rotate(rotation, glm::degrees(pitch), glm::vec3(1, 0, 0));

    updateVectors();
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getDirection() const {
    return front;
}

glm::mat4 Camera::getProjection() const {
    const float aspect = static_cast<float>(window->width) / static_cast<float>(window->height);
    return glm::perspective(fov, aspect, 0.01f, 1000.0f);
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getMatrixMultiply() const {
    return getProjection() * getView();
}
