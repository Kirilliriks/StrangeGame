//
// Created by KirilliriK on 28.05.2023.
//

#include "Input.hpp"

glm::vec3 Input::mousePos = glm::vec3(0, 0, 0);
glm::vec3 Input::lastMousePos = glm::vec3(0, 0, 0);
std::unordered_map<int, Input::Key*> Input::keys;
std::unordered_map<int, Input::Button*> Input::buttons;

Input::Key &Input::enter = registerKey(new Input::Key(GLFW_KEY_ENTER));
Input::Key &Input::tab = registerKey(new Input::Key(GLFW_KEY_TAB));
Input::Key &Input::leftShift = registerKey(new Input::Key(GLFW_KEY_LEFT_SHIFT));

Input::Button &Input::leftClick = registerButton(new Input::Button(GLFW_MOUSE_BUTTON_LEFT));
Input::Button &Input::rightClick = registerButton(new Input::Button(GLFW_MOUSE_BUTTON_RIGHT));