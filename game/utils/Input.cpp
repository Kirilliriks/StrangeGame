//
// Created by KirilliriK on 28.05.2023.
//

#include "Input.hpp"

glm::vec3 Input::mousePos = glm::vec3(0);
glm::vec3 Input::lastMousePos = glm::vec3(0);
Input::InputKey keys[GLFW_KEY_LAST + 1] = { };

Input::InputKey &Input::ENTER = registerKey(GLFW_KEY_ENTER);
Input::InputKey &Input::TAB = registerKey(GLFW_KEY_TAB);
Input::InputKey &Input::LEFT_SHIFT = registerKey(GLFW_KEY_LEFT_SHIFT);

Input::InputKey &Input::Q = registerKey(GLFW_KEY_Q);
Input::InputKey &Input::E = registerKey(GLFW_KEY_E);
Input::InputKey &Input::F = registerKey(GLFW_KEY_F);
Input::InputKey &Input::T = registerKey(GLFW_KEY_T);

Input::InputKey &Input::LEFT_CLICK = registerKey(GLFW_MOUSE_BUTTON_LEFT);
Input::InputKey &Input::RIGHT_CLICK = registerKey(GLFW_MOUSE_BUTTON_RIGHT);