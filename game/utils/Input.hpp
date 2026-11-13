//
// Created by KirilliriK on 01.03.2022.
//

#ifndef INPUT_H
#define INPUT_H

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "imgui_impl_glfw.h"

class Input {
public:
    struct InputKey {
        inline static int KEY_INDEX_COUNTER = 0;

        const int code = KEY_INDEX_COUNTER++;
        bool down = false;
        bool pressed = false;
        bool wasDown = false;

        void tick() {
            pressed = !wasDown && down;
            wasDown = down;
        }
    };

    static glm::vec3 mousePos;
    static glm::vec3 lastMousePos;
    static InputKey keys[];

    static InputKey& ENTER;
    static InputKey& TAB;
    static InputKey& LEFT_SHIFT;
    static InputKey& Q;
    static InputKey& E;
    static InputKey& F;
    static InputKey& T;

    static InputKey& LEFT_CLICK;
    static InputKey& RIGHT_CLICK;

    static void tick() {
        for (InputKey& key: keys) {
            key.tick();
        }
    }

    static InputKey& registerKey(const int& code) {
        return keys[code];
    }

    explicit Input(GLFWwindow* window) {
        glfwSetKeyCallback(
            window,
            [](GLFWwindow*, const int key, const int, const int action, const int) {
                const bool act = action != GLFW_RELEASE;
                keys[key].down = act;
            }
        );

        glfwSetMouseButtonCallback(
            window,
            [](GLFWwindow* callbackWindow, const int button, const int action, const int mods) {
                ImGui_ImplGlfw_MouseButtonCallback(callbackWindow, button, action, mods);

                const bool act = action != GLFW_RELEASE;
                keys[button].down = act;
            }
        );
    }
};

#endif //INPUT_H
