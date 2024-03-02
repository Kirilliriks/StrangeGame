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

    const glm::ivec3 v = octree.voxelRaycastDDA(camera.getDirection(), camera.getPosition(), 100).voxelPos;
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
            const TraceStack traceStack = voxelRaycast(camera.getDirection(), camera.getPosition(), 500);
            game->getPolygonRenderer()->traceLine(traceStack, traceStack);
        }
    }
}

static int cameraSpeed[] = {30};

void World::imgui(const double& deltaTime) {
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);

    ImGui::Text("Position x=%d y=%d z=%d",
        static_cast<int>(camera.getX()),
        static_cast<int>(camera.getY()),
        static_cast<int>(camera.getZ())
    );
    ImGui::Text("Camera direction x=%.2f y=%.2f z=%.2f",
        camera.getDirection().x,
        camera.getDirection().y,
        camera.getDirection().z
    );
    ImGui::Text("Camera angels yaw=%d pitch=%d",
        static_cast<int>(camera.getYaw()),
        static_cast<int>(camera.getPitch())
    );
    ImGui::Text("Voxel x=%d y=%d z=%d",
        frontVoxel.x,
        frontVoxel.y,
        frontVoxel.z
    );
    ImGui::Text("Distance %f", traceCast.distance);

    ImGui::ColorPicker3("Choose voxel color", editColor);

    if (ImGui::InputInt("Camera speed", cameraSpeed)) {
        camera.setSpeed(cameraSpeed[0]);
    }

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

TraceStack World::voxelRaycast(
    const glm::vec3& rayDirection,
    const glm::vec3& startPosition,
    float maxDistance
) const {
    TraceStack traceStack;

    const float octreeSize = octree.getSize();
    const auto step = glm::ivec3(glm::sign(rayDirection));

    const glm::vec3 rayStepSizeSingle = 1.0f / glm::max(glm::abs(rayDirection), 0.001f);
    const glm::vec3 rayStepSize = rayStepSizeSingle * octreeSize;

    auto voxelPos = glm::ivec3(startPosition / octreeSize);
    if (startPosition.x < 0) {
        voxelPos.x--;
    }

    if (startPosition.y < 0) {
        voxelPos.y--;
    }

    if (startPosition.z < 0) {
        voxelPos.z--;
    }

    const float halfOctreeSize = octreeSize / 2.0f;
    auto rayLength = -(glm::sign(rayDirection) * (glm::mod(startPosition, octreeSize) - halfOctreeSize) - halfOctreeSize) * rayStepSizeSingle;

    glm::bvec3 mask;

    float distance = 0.0f;
    while (distance < maxDistance) {
        if (voxelPos == glm::ivec3(0)) {
            return octree.voxelRaycastTraversal(rayDirection, startPosition + rayDirection * distance);
        }

        mask = glm::lessThanEqual(
            rayLength,
            glm::min(
                glm::vec3(rayLength.y, rayLength.z, rayLength.x),
                glm::vec3(rayLength.z, rayLength.x, rayLength.y)
            )
        );

        rayLength += glm::vec3(mask) * rayStepSize;
        distance = glm::length(glm::vec3(mask) * (rayLength - rayStepSize));
        voxelPos += glm::ivec3(glm::vec3(mask)) * step;

        traceStack.entryStack.push_back(startPosition + rayDirection * distance);
    }

    return traceStack;
}
