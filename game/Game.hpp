//
// Created by KirilliriK on 28.02.2022.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "render/polygon/PolygonRender.hpp"
#include "render/voxel/VoxelRender.hpp"
#include "utils/Input.hpp"

class VoxelRender;
class World;
class Game {
public:
    static bool focused;
    static bool debugRender;

    explicit Game(Window *window);
    void update(double deltaTime) const;
    void render(double deltaTime) const;
    void imgui(double deltaTime) const;

    Window *getWindow() const;
    World *getWorld() const;
    VoxelRender *getRenderer() const;
    PolygonRender *getPolygonRenderer() const;
private:
    Window *window;
    Input *input;
    World *world;
    VoxelRender *renderer;
    PolygonRender *polygonRenderer;
};


#endif //GAME_GAME_H
