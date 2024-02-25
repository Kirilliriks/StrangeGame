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
    camera.update(deltaTime);

    traceCast = octree.voxelRaycastDDA(camera.getDirection(), camera.getPosition(), 100);
    const glm::ivec3 v = traceCast.voxelPos; //octree.voxelRaycast(camera.getDirection(), camera.getPosition(), 500);
    frontVoxel = glm::vec3(v);

    if (!Game::focused) {
        return;
    }

    if (!Game::debugRender) {
        if (Input::LEFT_CLICK.pressed || (Input::LEFT_CLICK.down && Input::LEFT_SHIFT.down)) {
            octree.setVoxel(traceCast.preVoxelPos, glm::vec4(editColor[0], editColor[1], editColor[2], 255));
            game->getRenderer()->updateWorld();
        }

        if (Input::RIGHT_CLICK.pressed || (Input::RIGHT_CLICK.down && Input::LEFT_SHIFT.down)) {
            octree.removeVoxel(v);
            game->getRenderer()->updateWorld();
        }
    } else {
        if (Input::LEFT_CLICK.pressed) {
            const TraceStack testStack = octree.voxelRaycastTraversalTest(camera.getDirection(), camera.getPosition());
            const TraceStack traceStack = octree.voxelRaycastTraversal(camera.getDirection(), camera.getPosition());
            game->getPolygonRenderer()->traceLine(testStack, traceStack);
        }
    }
}

void World::imgui(const double& deltaTime) const {
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);
    ImGui::Text("Cam x=%d y=%d z=%d", (int) camera.getX(), (int) camera.getY(), (int) camera.getZ());
    ImGui::Text("Cam yaw=%d pitch=%d", (int) camera.getYaw(), (int) camera.getPitch());
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
