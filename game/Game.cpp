//
// Created by KirilliriK on 28.02.2022.
//
#include "Game.h"

Game::Game(Window *window) : camera(window) {
    this->window = window;

    renderer = new VoxelRender(this);
    renderer->createWorld();
}

void Game::update(double deltaTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(window->getGLWindow(), &mouseX, &mouseY);
    camera.update(deltaTime, (float)mouseX, (float)mouseY);
}

void Game::render(double deltaTime) {
    renderer->render(deltaTime);
}

Camera &Game::getCamera() {
    return camera;
}

Window *Game::getWindow() {
    return window;
}
