//
// Created by KirilliriK on 28.02.2022.
//
#include "Game.hpp"

bool Game::focused = true;
bool Game::debugRender = true;

Game::Game(Window *window) : window(window) {
    Avox::Options::loadOptions();

    input = new Input(window->getGLWindow());
    world = new World(this);
    renderer = new VoxelRender(this);
    polygonRenderer = new PolygonRender(this);
    world->createWorld();
    renderer->updateWorld();
    //polygonRenderer->rebuildWorld();
}

void Game::update(const double& deltaTime) const {
    world->update(deltaTime);
    //polygonRenderer->update();
}

void Game::render(const double& deltaTime) const {
    renderer->render(deltaTime);
    //polygonRenderer->render(deltaTime);
}

void Game::imgui(const double& deltaTime) const {
    world->imgui(deltaTime);
    //polygonRenderer->imgui();
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

PolygonRender* Game::getPolygonRenderer() const {
    return polygonRenderer;
}
