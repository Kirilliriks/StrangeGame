//
// Created by KirilliriK on 23.05.2023.
//

#include "World.hpp"
#include "imgui.h"

static float editColor[] = {0, 0, 0};

World::World(Game* game) : game(game), camera(game->getWindow()) {
}

void World::update(const double& deltaTime) {
    camera.update(deltaTime);

    //const glm::ivec3 v = octreeSpace.voxelRaycast(camera.getDirection(), camera.getPosition(), 100).voxelPos;
    //frontVoxel = glm::vec3(v);

    if (!Game::focused) {
        return;
    }

    if (!Game::debugRender) {
        if (Input::LEFT_CLICK.pressed || (Input::LEFT_CLICK.down && Input::LEFT_SHIFT.down)) {
            //octree.setVoxel(traceCast.preVoxelPos, glm::vec4(editColor[0], editColor[1], editColor[2], 255));
            game->getRenderer()->updateWorld();
        }

        if (Input::RIGHT_CLICK.pressed || (Input::RIGHT_CLICK.down && Input::LEFT_SHIFT.down)) {
            //octree.removeVoxel(v);
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
    const auto octreee = octreeSpace.getOctreePosition(glm::ivec3(camera.getPosition()));
    ImGui::Text("Octree x=%d y=%d z=%d",
        static_cast<int>(octreee.x),
        static_cast<int>(octreee.y),
        static_cast<int>(octreee.z)
    );
    ImGui::Text("Distance %f", traceCast.distance);

    ImGui::ColorPicker3("Choose voxel color", editColor);

    if (ImGui::InputInt("Camera speed", cameraSpeed)) {
        camera.setSpeed(cameraSpeed[0]);
    }

    ImGui::End();
}

void World::setVoxel(const glm::ivec3&vec, const glm::vec4&color) {
    octreeSpace.setVoxel(vec, color);
}

void World::createWorld() {
    octreeSpace.updateSpaceCenter(glm::ivec3(camera.getPosition()), true);
}


OctreeSpace& World::getOctreeSpace() {
    return octreeSpace;
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
    const float maxDistance
) {
    return octreeSpace.voxelRaycast(rayDirection, startPosition, maxDistance);
}
