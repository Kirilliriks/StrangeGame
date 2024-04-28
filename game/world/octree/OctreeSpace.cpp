//
// Created by KirilliriK on 20.04.2024.
//

#include "OctreeSpace.hpp"

#include <gl.h>
#include <iostream>
#include <random>
#include <glm/gtc/noise.hpp>
#include <utility>

#define OGT_VOX_IMPLEMENTATION
#include "../dependency/ogt/ogt_vox.h"

OctreeSpace::OctreeSpace(const int& radius) : radius(radius), diameter(radius * 2 + 1), dataSize(0) {
    octrees.resize(diameter * diameter * diameter);

    for (int z = 0; z < diameter; z++) {
        for (int y = 0; y < diameter; y++) {
            for (int x = 0; x < diameter; x++) {
                octrees[x + (z * diameter + y) * diameter] = std::make_shared<Octree>(octreeSideSize, maxDepth);
            }
        }
    }
}

void OctreeSpace::updateSpaceCenter(const glm::ivec3& position, const bool& updateOctrees) {
    const glm::ivec3 newCenter = getOctreePosition(position);
    if (spaceCenter == newCenter) {
        return;
    }

    spaceCenter = newCenter;

    if (updateOctrees) {
        this->generateNoiseOctrees();
    }
}

void OctreeSpace::setVoxel(const glm::ivec3& position, const glm::vec4& color) {
    const glm::ivec3 octreePosition = getOctreePosition(position);

    const std::shared_ptr<Octree> octree = getOctree(octreePosition);
    if (!octree) {
        return;
    }

    const glm::ivec3 localPosition = position - octreePosition * octreeSideSize;
    octree->setVoxel(localPosition, color);
}

void OctreeSpace::setOctree(const glm::ivec3& position, std::shared_ptr<Octree> octree) {
    glm::ivec3 relativeOctree = spaceCenter - position;
    if (relativeOctree.x < radius || relativeOctree.x > radius ||
        relativeOctree.y < radius || relativeOctree.y > radius ||
        relativeOctree.z < radius || relativeOctree.z > radius
    ) {
        return;
    }

    relativeOctree += radius;
    octrees[relativeOctree.x + (relativeOctree.z * diameter + relativeOctree.y) * diameter] = std::move(octree);
}


std::shared_ptr<Octree> OctreeSpace::getOctree(const glm::ivec3& position, const bool& local) {
    glm::ivec3 relativeOctree = local || !spaceShift ? position : spaceCenter - position;
    if (relativeOctree.x < -radius || relativeOctree.x > radius ||
        relativeOctree.y < -radius || relativeOctree.y > radius ||
        relativeOctree.z < -radius || relativeOctree.z > radius
    ) {
        return {nullptr};
    }

    relativeOctree += radius;

    return octrees[relativeOctree.x + (relativeOctree.z * diameter + relativeOctree.y) * diameter];
}

glm::ivec3 OctreeSpace::getOctreePosition(const glm::ivec3& position) const {
    return glm::ivec3(glm::floor(glm::vec3(position) / static_cast<float>(octreeSideSize)));
}

int OctreeSpace::getOctreeSideSize() const {
    return octreeSideSize;
}

int OctreeSpace::getRadius() const {
    return radius;
}

int OctreeSpace::getDiameter() const {
    return diameter;
}

glm::ivec3 OctreeSpace::getSpaceCenter() const {
    return spaceCenter;
}

void OctreeSpace::fillBuffers(const int& worldBuffer, const int& matrixBuffer) {
    std::vector<int> ids;
    ids.resize(diameter * diameter * diameter, -1);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, worldBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, nullptr, GL_STATIC_DRAW);

    int lastNodesCount = 0;
    int offset = 0;
    for (int z = 0; z < diameter; z++) {
        for (int y = 0; y < diameter; y++) {
            for (int x = 0; x < diameter; x++) {
                const auto octree = getOctree(glm::ivec3(x, y, z) - radius, true);

                const int nodesCount = octree->nodesCount();
                const int size = nodesCount * sizeof(Node);

                glBufferSubData(
                    GL_SHADER_STORAGE_BUFFER,
                    offset,
                    size,
                    octree->getData()
                );

                offset += size;
                ids[x + (z * diameter + y) * diameter] = nodesCount > 1 ? lastNodesCount : -1;
                lastNodesCount += nodesCount;
            }
        }
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, worldBuffer);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, matrixBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, ids.size() * sizeof(int), ids.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, matrixBuffer);
}

TraceStack OctreeSpace::voxelRaycast(
    const glm::vec3& rayDirection,
    const glm::vec3& startPosition,
    float maxDistance
) {
    TraceStack traceStack;

    const auto octreeSize = static_cast<float>(octreeSideSize);
    const auto step = glm::ivec3(glm::sign(rayDirection));

    const glm::vec3 rayStepSizeSingle = 1.0f / glm::max(glm::abs(rayDirection), 0.001f);
    const glm::vec3 rayStepSize = rayStepSizeSingle * octreeSize;

    auto rayOctreePosition = glm::ivec3(glm::floor(startPosition / octreeSize));

    // std::cout << "SP x=" << startPosition.x << " y=" << startPosition.y << " z=" << startPosition.z << std::endl;
    // std::cout << "SP / OS x=" << startPositionDivideOctreePosition.x << " y=" << startPositionDivideOctreePosition.y << " z=" << startPositionDivideOctreePosition.z << std::endl;
    // std::cout << "OP x=" << octreePosition.x << " y=" << octreePosition.y << " z=" << octreePosition.z << std::endl;
    // std::cout << "OS " << octreeSize << std::endl;

    const float halfOctreeSize = octreeSize / 2;
    auto rayLength = -(glm::sign(rayDirection) * (glm::mod(startPosition, octreeSize) - halfOctreeSize) - halfOctreeSize) * rayStepSizeSingle;

    glm::bvec3 mask;

    float distance = 0.0f;
    while (distance < maxDistance) {
        if (const std::shared_ptr<Octree> octree = getOctree(rayOctreePosition)) {

            auto octreeGlobalPosition = glm::vec3(rayOctreePosition);
            octreeGlobalPosition *= octreeSideSize;

            const auto result = octree->voxelRaycastTraversal(rayDirection, startPosition + rayDirection * distance, octreeGlobalPosition);
            if (result.voxelPos != glm::ivec3(0)) {
                return result;
            }
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
        rayOctreePosition += glm::ivec3(glm::vec3(mask)) * step;

        traceStack.entryStack.push_back(startPosition + rayDirection * distance);
    }

    return traceStack;
}

void OctreeSpace::calculateDataSize() {
    for (int z = -radius; z <= radius; z++) {
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                const auto octree = getOctree(glm::ivec3(x, y, z), true);

                dataSize += octree->nodesCount() * sizeof(Node);
            }
        }
    }
}

void OctreeSpace::loadModels() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand(0, 100);

    const ogt_vox_scene* scene = load_vox_scene("vox/emil.vox");
    const ogt_vox_model* model = scene->models[0];
    const auto [color] = scene->palette;

    uint32_t voxel_index = 0;
    for (uint32_t z = 0; z < model->size_z; z++) {
        for (uint32_t y = 0; y < model->size_y; y++) {
            for (uint32_t x = 0; x < model->size_x; x++, voxel_index++) {

                const uint32_t color_index = model->voxel_data[voxel_index];
                if (color_index == 0) {
                    continue;
                }

                auto [r, g, b, a] = color[color_index];
                auto col = rand(rng);
                setVoxel(glm::ivec3(x, z - 480, y), glm::vec4(col, col, col, 255) / 255.0f);
            }
        }
    }
}

void OctreeSpace::generateNoiseOctrees() {
    dataSize = 0;

    for (int z = 0; z < diameter; z++) {
        const int realZ = spaceCenter.z + z;

        for (int y = 0; y < diameter; y++) {
            const int realY = spaceCenter.y + y;

            for (int x = 0; x < diameter; x++) {
                const int realX = spaceCenter.x + x;

                auto octree = octrees[x + (z * diameter + y) * diameter];

                if (y == 0) {
                    generateOctree(glm::ivec3(realX, radius / 2, realZ), octree);
                }
            }
        }
    }

    loadModels();

    calculateDataSize();
}

void OctreeSpace::generateOctree(const glm::ivec3& position, const std::shared_ptr<Octree>& octree) const {
    const int worldSize = octreeSideSize;
    for (int z = 0; z < worldSize; z++) {
        const auto realZ = static_cast<float>(position.z * octreeSideSize + z);

        for (int x = 0; x < worldSize; x++) {
            const auto realX = static_cast<float>(position.x * octreeSideSize + x);

            constexpr float divider = 32.0f;
            const float per = (glm::simplex(glm::vec3(realX / divider, realZ / divider, 21)) + 1) / 2.0f;

            const int y = static_cast<int>(per * divider);
            if (y < 0 || y > worldSize) continue;

            for (int i = 0; i <= y; i++) {
                octree->setVoxel(glm::ivec3(x, i, z), glm::vec4(per, per, per, 1.0f));
            }
        }
    }
}
