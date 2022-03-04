#include <iostream>

#include <glad.h>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

#include "game/Game.h"
#include "game/render/Window.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

int main() {
    Window *window = new Window();

    GLFWwindow *glWindow = window->getGLWindow();
    Game *game = new Game(window);

    double deltaTime;
    double lastFrame = 0;
    while(glfwWindowShouldClose(glWindow) == GL_FALSE) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor4f(0, 0, 0, 0);

        game->update(deltaTime);
        ImGui::Render();
        game->render(deltaTime);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(glWindow);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glWindow);
    glfwTerminate();

    delete game;
    delete window;
    return 0;
}