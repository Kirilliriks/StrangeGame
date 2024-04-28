//
// Created by KirilliriK on 01.03.2022.
//
#include <gl.h>
#include <iostream>

#include "Window.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../options/Options.hpp"

Window::Window() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    if (!Avox::Options::forceResolution) {
        width = mode->width;
        height = mode->height;
    } else {
        width = Avox::Options::width;
        height = Avox::Options::height;
    }

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_MAXIMIZED, true);

    glWindow = glfwCreateWindow(mode->width, mode->height, "StrangeGame",
                                nullptr, nullptr);
    if (glWindow == nullptr) {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(glWindow);
    glfwSwapInterval(1); // Vsync
    glfwShowWindow(glWindow);

    //glfwSetWindowSize(glWindow, mode->width, mode->height); // TODO POLYGONAL
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glWindow, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DOUBLEBUFFER);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, mode->width, mode->height);

    const std::string title("Avox");
    glfwSetWindowTitle(glWindow, title.c_str());
}

GLFWwindow* Window::getGLWindow() const {
    return glWindow;
}

void Window::setTitle(const std::string& title) const {
    glfwSetWindowTitle(glWindow, title.c_str());
}
