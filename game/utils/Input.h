//
// Created by KirilliriK on 01.03.2022.
//

#ifndef INPUT_H
#define INPUT_H

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "imgui_impl_glfw.h"
#include <unordered_map>
#include <iostream>
#include <string>

class Input {
public:
    struct Key {
        int code;
        bool down = false;
        bool pressed = false;
        bool wasDown = false;

        explicit Key(int code) : code(code) { }

        void tick() {
            pressed = !wasDown && down;
            wasDown = down;
        }
    };

    struct Button {
        int code;
        bool down = false;
        bool pressed = false;
        bool wasDown = false;

        explicit Button(int code) : code(code) { }

        void tick() {
            pressed = !wasDown && down;
            wasDown = down;
        }
    };

    static glm::vec3 mousePos;
    static glm::vec3 lastMousePos;
    static std::unordered_map<int, Key*> keys;
    static std::unordered_map<int, Button*> buttons;

    static Key &enter;
    static Key &tab;
    static Key &leftShift;

    static Button &leftClick;
    static Button &rightClick;

    Input(GLFWwindow *window) {
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto pair = keys.find(key);
            if (pair != keys.end()) {
                const bool act = action != GLFW_RELEASE;
                pair->second->down = act;
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

            auto pair = buttons.find(button);
            if (pair != buttons.end()) {
                const bool act = action != GLFW_RELEASE;
                pair->second->down = act;
            }
        });
    }

    static void tick() {
        for (auto pair: keys) {
            pair.second->tick();
        }

        for (auto pair: buttons) {
            pair.second->tick();
        }
    }

    static Key &registerKey(Key *key) {
        keys[key->code] = key;
        return *key;
    }

    static Button &registerButton(Button *button) {
        buttons[button->code] = button;
        return *button;
    }
};

#endif //INPUT_H