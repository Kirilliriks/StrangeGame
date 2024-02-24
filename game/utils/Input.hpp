//
// Created by KirilliriK on 01.03.2022.
//

#ifndef INPUT_H
#define INPUT_H

#include <ranges>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "imgui_impl_glfw.h"
#include <unordered_map>

class Input {
public:
    struct Key {
        const int code;
        bool down = false;
        bool pressed = false;
        bool wasDown = false;

        explicit Key(const int& code) : code(code) { }

        void tick() {
            pressed = !wasDown && down;
            wasDown = down;
        }
    };

    struct Button {
        const int code;
        bool down = false;
        bool pressed = false;
        bool wasDown = false;

        explicit Button(const int& code) : code(code) { }

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
    static Key &q;
    static Key &e;
    static Key &f;

    static Button &leftClick;
    static Button &rightClick;

    explicit Input(GLFWwindow *window) {
        glfwSetKeyCallback(window, [](GLFWwindow* _, const int key, const int, const int action, const int) {
            if (const auto pair = keys.find(key); pair != keys.end()) {
                const bool act = action != GLFW_RELEASE;
                pair->second->down = act;
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, const int button, const int action, const int mods) {
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

            if (const auto pair = buttons.find(button); pair != buttons.end()) {
                const bool act = action != GLFW_RELEASE;
                pair->second->down = act;
            }
        });
    }

    static void tick() {
        for (Key* key : std::ranges::views::values(keys)) {
            key->tick();
        }

        for (Button* button: std::ranges::views::values(buttons)) {
            button->tick();
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