//
// Created by KirilliriK on 23.05.2023.
//

#include "World.h"
#include <random>
#include "glm/gtc/noise.hpp"

World::World(Game *game) : game(game), camera(game->getWindow()) {

}

void World::update(double deltaTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(game->getWindow()->getGLWindow(), &mouseX, &mouseY);
    camera.update(deltaTime, (float)mouseX, (float)mouseY);

    debugCast = octree.raycastVoxel(camera.getDirection(), camera.getPosition());
    const glm::ivec3 v = debugCast.voxelPos;//octree.voxelRaycast(camera.getDirection(), camera.getPosition(), 500);
    frontVoxel = glm::vec3(v);

    int state = glfwGetMouseButton(game->getWindow()->getGLWindow(), GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        octree.setVoxel(v, glm::vec4(255, 0, 0, 255));
        game->getRenderer()->updateWorld();
    }
}

void World::setVoxel(const glm::ivec3 &vec, const glm::vec4& color) {
    octree.setVoxel(vec, color);
}

void World::createWorld() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand(0,100);

    const int worldSize = octree.getSize();
    for (int z = 0; z < worldSize; z++) {
        for (int x = 0; x < worldSize; x++) {
            float per = glm::simplex(glm::vec3(x / 32.0f, z / 32.0f, 21));
            per = (per + 1) / 2;
            int y = (int)(per * (float)32);
            if (y < 0 || y > worldSize) continue;

            for (int i = 0; i <= y; i++) {
                setVoxel(glm::ivec3(x, i, z), glm::vec4(per, per, per, 1.0f));
            }
        }
    }

    std::cout << "Generation end, nodes count: " << octree.nodesCount() << std::endl;
}


Octree &World::getOctree() {
    return octree;
}

Camera &World::getCamera() {
    return camera;
}

Octree::DebugCast &World::getDebugCast() {
    return debugCast;
}

glm::ivec3 &World::getFrontVoxel() {
    return frontVoxel;
}
