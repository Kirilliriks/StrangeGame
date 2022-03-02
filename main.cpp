#include <iostream>

#include <glad.h>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

#include "game/Game.h"
#include "game/render/Window.h"
#include "game/world/Octree.h"

int main() {
    Octree octree;
    octree.setVoxel(glm::ivec3(0, 0, 0), Color(1, 1, 1));
    octree.setVoxel(glm::ivec3(128, 128, 128), Color(1, 1, 1));
    return 0;
    Window *window = new Window();

    GLFWwindow *glWindow = window->getGLWindow();
    Game *game = new Game(window);

    double deltaTime;
    double lastFrame = 0;
    while(glfwWindowShouldClose(glWindow) == GL_FALSE) {
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor4f(0, 0, 0, 0);

        game->update(deltaTime);
        game->render(deltaTime);

        glfwSwapBuffers(glWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(glWindow);
    glfwTerminate();

    delete game;
    delete window;
    return 0;
}