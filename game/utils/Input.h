//
// Created by KirilliriK on 01.03.2022.
//

#ifndef INPUT_H
#define INPUT_H

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
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

    static glm::vec3 mousePos;
    static glm::vec3 lastMousePos;
    static Key &enter;
    static std::unordered_map<int, Key*> keys;

    Input(GLFWwindow *window) {
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto pair = keys.find(key);
            if (pair != keys.end()) {
                const bool act = action != GLFW_RELEASE;
                pair->second->down = act;
            }
        });
    }

    static void tick() {
        for (auto pair: keys) {
            pair.second->tick();
        }
    }

    static Key &registerKey(Key *key) {
        keys[key->code] = key;
        return *key;
    }
};

#endif //INPUT_H