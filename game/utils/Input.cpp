//
// Created by KirilliriK on 28.05.2023.
//

#include "Input.hpp"

glm::vec3 Input::mousePos = glm::vec3(0);
glm::vec3 Input::lastMousePos = glm::vec3(0);
std::unordered_map<int, Input::Key*> Input::keys;
std::unordered_map<int, Input::Button*> Input::buttons;

Input::Key &Input::ENTER = registerKey(new Key(GLFW_KEY_ENTER));
Input::Key &Input::TAB = registerKey(new Key(GLFW_KEY_TAB));
Input::Key &Input::LEFT_SHIFT = registerKey(new Key(GLFW_KEY_LEFT_SHIFT));

Input::Key &Input::Q = registerKey(new Key(GLFW_KEY_Q));
Input::Key &Input::E = registerKey(new Key(GLFW_KEY_E));
Input::Key &Input::F = registerKey(new Key(GLFW_KEY_F));
Input::Key &Input::T = registerKey(new Key(GLFW_KEY_T));

Input::Button &Input::LEFT_CLICK = registerButton(new Button(GLFW_MOUSE_BUTTON_LEFT));
Input::Button &Input::RIGHT_CLICK = registerButton(new Button(GLFW_MOUSE_BUTTON_RIGHT));