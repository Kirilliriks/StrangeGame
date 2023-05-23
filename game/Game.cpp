//
// Created by KirilliriK on 28.02.2022.
//
#include "Game.h"

Game::Game(Window *window) : window(window) {
    world = new World(this);
    renderer = new VoxelRender(this);
    world->createWorld();
    renderer->updateWorld();
}

void Game::update(double deltaTime) {
    world->update(deltaTime);
}

void Game::render(double deltaTime) {
    renderer->render(deltaTime);
}

void Game::imgui(double deltaTime) {
    world->imgui(deltaTime);
}

Window *Game::getWindow() {
    return window;
}

World *Game::getWorld() {
    return world;
}

VoxelRender *Game::getRenderer() {
    return renderer;
}
