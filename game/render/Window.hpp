//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_WINDOW_H
#define STRANGEGAME_WINDOW_H

#include <iostream>
#include "../../glfw/include/GLFW/glfw3.h"

class Window {
public:
    Window();
    GLFWwindow *getGLWindow() const;
    void setTitle(const std::string& title) const;

    int width, height;

    double getHalfWidth() const {
        return width / 2.0;
    }

    double getHalfHeight() const {
        return height / 2.0;
    }

private:
    GLFWwindow *glWindow;
};


#endif //STRANGEGAME_WINDOW_H
