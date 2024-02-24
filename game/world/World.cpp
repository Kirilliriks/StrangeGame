//
// Created by KirilliriK on 23.05.2023.
//

#include "World.hpp"
#include <random>
#include "glm/gtc/noise.hpp"
#include "imgui.h"

static float editColor[] = {0, 0, 0};

World::World(Game* game) : game(game), camera(game->getWindow()) {
}

void World::update(const double& deltaTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(game->getWindow()->getGLWindow(), &mouseX, &mouseY);
    camera.update(deltaTime, static_cast<float>(mouseX), static_cast<float>(mouseY));

    traceCast = octree.voxelRaycastDDA(camera.getDirection(), camera.getPosition(), 100);
    const glm::ivec3 v = traceCast.voxelPos; //octree.voxelRaycast(camera.getDirection(), camera.getPosition(), 500);
    frontVoxel = glm::vec3(v);

    if (!Game::focused) {
        return;
    }

    if (!Game::debugRender) {
        if (Input::leftClick.pressed || (Input::leftClick.down && Input::leftShift.down)) {
            octree.setVoxel(traceCast.preVoxelPos, glm::vec4(editColor[0], editColor[1], editColor[2], 255));
            game->getRenderer()->updateWorld();
        }

        if (Input::rightClick.pressed || (Input::rightClick.down && Input::leftShift.down)) {
            octree.removeVoxel(v);
            game->getRenderer()->updateWorld();
        }
    }
    else {
        if (Input::leftClick.pressed) {
            const TraceStack traceStack = octree.voxelRaycastTraversal(camera.getDirection(), camera.getPosition());
            game->getPolygonRenderer()->traceLine(traceStack);
        }
    }
}

void World::imgui(const double& deltaTime) const {
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);
    ImGui::Text("Cam x=%i y=%i z=%i", camera.getX(), camera.getY(), camera.getZ());
    ImGui::Text("Cam yaw=%i pitch=%i", camera.getYaw(), camera.getPitch());
    ImGui::Text("Voxel x=%d y=%d z=%d", frontVoxel.x, frontVoxel.y, frontVoxel.z);
    ImGui::Text("Try x=%d y=%d z=%d", traceCast.voxelPos.x, traceCast.voxelPos.y, traceCast.voxelPos.z);
    ImGui::Text("Distance %f", traceCast.distance);

    ImGui::ColorPicker3("Choose voxel color", editColor);
    ImGui::End();
}

void World::setVoxel(const glm::ivec3&vec, const glm::vec4&color) {
    octree.setVoxel(vec, color);
}

void World::createWorld() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand(0, 100);

    const int worldSize = octree.getSize();
    for (int z = 0; z < worldSize; z++) {
        for (int x = 0; x < worldSize; x++) {
            constexpr float divider = 32.0f;
            float per = glm::simplex(glm::vec3(x / divider, z / divider, 21));
            per = (per + 1) / 2.0f;

            const int y = static_cast<int>(per * divider);
            if (y < 0 || y > worldSize) continue;

            for (int i = 0; i <= y; i++) {
                setVoxel(glm::ivec3(x, i, z), glm::vec4(per, per, per, 1.0f));
            }
        }
    }

    std::cout << "Generation end, nodes count: " << octree.nodesCount() << std::endl;
}


Octree& World::getOctree() {
    return octree;
}

Camera& World::getCamera() {
    return camera;
}

TraceStack& World::getTraceCast() {
    return traceCast;
}

glm::ivec3& World::getFrontVoxel() {
    return frontVoxel;
}
