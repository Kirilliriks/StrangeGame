#include <glad.h>
#include "GLFW/glfw3.h"

#include "game/Game.hpp"
#include "game/render/Window.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

int main() {
    const auto window = new Window();
    const Game *game = new Game(window);

    GLFWwindow *glWindow = window->getGLWindow();

    double lastFrame = 0;
    while(glfwWindowShouldClose(glWindow) == GL_FALSE) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const double currentFrame = glfwGetTime();
        const double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Input::tick();
        glfwPollEvents();
        game->update(deltaTime); //Update logic + physics

        game->render(deltaTime); //Render voxels
        game->imgui(deltaTime);  //Render develop GUI
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(glWindow);
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