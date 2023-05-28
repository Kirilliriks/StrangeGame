//
// Created by KirilliriK on 28.05.2023.
//

#include "Input.h"

glm::vec3 Input::mousePos = glm::vec3(0, 0, 0);
glm::vec3 Input::lastMousePos = glm::vec3(0, 0, 0);
std::unordered_map<int, Input::Key*> Input::keys;
Input::Key &Input::enter = registerKey(new Input::Key(GLFW_KEY_ENTER));