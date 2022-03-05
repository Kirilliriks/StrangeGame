//
// Created by KirilliriK on 28.02.2022.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <glad.h>
#include "GLFW/glfw3.h"
#include "render/VoxelRender.h"
#include "render/Camera.h"
#include "render/Window.h"

class VoxelRender;
class Game {
public:
    Game(Window *window);
    void update(double deltaTime);
    void render(double deltaTime);
    void imgui(double deltaTime);

    Camera& getCamera();
    Window *getWindow();
private:
    VoxelRender *renderer;
    Window *window;

    Camera camera;
};


#endif //GAME_GAME_H
