//
// Created by KirilliriK on 28.02.2022.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "render/voxel/VoxelRender.h"
#include "utils/Input.h"
#include "render/Window.h"
#include "world/World.h"

class VoxelRender;
class World;
class Game {
public:
    Game(Window *window);
    void update(double deltaTime);
    void render(double deltaTime);
    void imgui(double deltaTime);

    Window *getWindow();
    World *getWorld();
    VoxelRender *getRenderer();

    static bool focused;
private:
    Window *window;
    Input *input;
    World *world;
    VoxelRender *renderer;
};


#endif //GAME_GAME_H
