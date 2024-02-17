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
    position = glm::vec3(18, 10, 18);
}

void Camera::update(double deltaTime, float mouseX, float mouseY) {
    glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 movementVector = direction;

    // Keys management
    GLFWwindow *glWindow = window->getGLWindow();

    if (Game::focused) {
        int state = glfwGetKey(glWindow, GLFW_KEY_W);
        if (state == GLFW_PRESS) {
            movement = speed * movementVector;
        }

        state = glfwGetKey(glWindow, GLFW_KEY_S);
        if (state == GLFW_PRESS) {
            movement = -speed * movementVector;
        }

        state = glfwGetKey(glWindow, GLFW_KEY_A);
        if (state == GLFW_PRESS) {
            movement = speed * glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        }

        state = glfwGetKey(glWindow, GLFW_KEY_D);
        if (state == GLFW_PRESS) {
            movement = speed * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
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

        yaw = (mouseX - (float) window->width / 2.0f) * 0.005f;
        pitch = (mouseY - (float) window->height / 2.0f) * 0.005f;

        direction = getDirection();
    }

    if (Input::tab.pressed) {
        Game::focused = !Game::focused;

        if (Game::focused) {
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;            // Disable Mouse
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard; // Disable Keyboard
            glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;            // Enable Mouse
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard
            glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
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

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getDirection() {
    glm::mat4 rota(1.0f);
    glm::mat4 rx = glm::rotate(rota, glm::degrees(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ry = glm::rotate(rota, glm::degrees(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    return glm::vec3(rx * ry * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}
