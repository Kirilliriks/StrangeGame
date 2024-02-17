//
// Created by KirilliriK on 28.02.2022.
//
#include "Game.hpp"

bool Game::focused = true;

Game::Game(Window *window) : window(window) {
    input = new Input(window->getGLWindow());
    world = new World(this);
    renderer = new VoxelRender(this);
    world->createWorld();
    renderer->updateWorld();
}

void Game::update(const double deltaTime) const {
    world->update(deltaTime);
}

void Game::render(const double deltaTime) const {
    renderer->render(deltaTime);
}

void Game::imgui(const double deltaTime) const {
    world->imgui(deltaTime);
}

Window *Game::getWindow() const {
    return window;
}

World *Game::getWorld() const {
    return world;
}

VoxelRender *Game::getRenderer() const {
    return renderer;
}
