//
// Created by KirilliriK on 20.04.2024.
//

#include "OctreeSpace.hpp"

#include <gl.h>
#include <iostream>
#include <bits/random.h>
#include <glm/gtc/noise.hpp>
#include <utility>

OctreeSpace::OctreeSpace(const int& radius) : radius(radius), diameter(radius * 2 + 1), dataSize(0) {
    octrees.resize(diameter * diameter * diameter, std::shared_ptr<Octree>(nullptr));
}

void OctreeSpace::updateSpaceCenter(const glm::ivec3& position) {
    const glm::ivec3 newCenter = getOctreePosition(position);
    if (spaceCenter == newCenter) {
        return;
    }

    spaceCenter = newCenter;
    updateOctrees();
}

void OctreeSpace::setVoxel(const glm::ivec3& position, const glm::vec4& color) {
    const glm::ivec3 octreePosition = getOctreePosition(position);

    const std::shared_ptr<Octree> octree = getOctree(octreePosition);
    if (!octree) {
        return;
    }

    octree->setVoxel(position, color);
}

void OctreeSpace::setOctree(const glm::ivec3& position, std::shared_ptr<Octree> octree) {
    glm::ivec3 relativeOctree = spaceCenter - position;
    if (relativeOctree.x < radius || relativeOctree.x > radius ||
        relativeOctree.y < radius || relativeOctree.y > radius ||
        relativeOctree.z < radius || relativeOctree.z > radius) {
        return;
    }

    relativeOctree += radius;
    octrees[relativeOctree.x + (relativeOctree.z * diameter + relativeOctree.y) * diameter] = std::move(octree);
}


std::shared_ptr<Octree> OctreeSpace::getOctree(const glm::ivec3& position, const bool& local) {
    glm::ivec3 relativeOctree = local ? position : spaceCenter - position;
    if (relativeOctree.x < -radius || relativeOctree.x > radius ||
        relativeOctree.y < -radius || relativeOctree.y > radius ||
        relativeOctree.z < -radius || relativeOctree.z > radius) {
        return {nullptr};
    }

    //std::cout << "REALLY " << relativeOctree.x << " " << relativeOctree.y << " " << relativeOctree.z << std::endl;
    relativeOctree += radius;
    //std::cout << "@ " << relativeOctree.x + (relativeOctree.y * diameter + relativeOctree.z) * diameter << std::endl;

    return octrees[relativeOctree.x + (relativeOctree.z * diameter + relativeOctree.y) * diameter];
}

glm::ivec3 OctreeSpace::getOctreePosition(const glm::ivec3& position) const {
    return position / octreeSideSize;
}

int OctreeSpace::getOctreeSideSize() const {
    return octreeSideSize;
}

int OctreeSpace::getRadius() const {
    return radius;
}

int OctreeSpace::getDiameter() const {
    return radius;
}

glm::ivec3 OctreeSpace::getSpaceCenter() const {
    return spaceCenter;
}

void OctreeSpace::fillBuffers(const int& worldBuffer, const int& matrixBuffer) {
    std::vector<int> ids;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, worldBuffer);

    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, nullptr, GL_STATIC_DRAW);

    int lastNodesCount = 0;
    int offset = 0;
    for (int z = -radius; z <= radius; z++) {
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                const auto octree = getOctree(glm::ivec3(x, y, z), true);

                const int nodesCount = octree->nodesCount();
                const int size = nodesCount * sizeof(Node);

                glBufferSubData(
                    GL_SHADER_STORAGE_BUFFER,
                    offset,
                    size,
                    octree->getData()
                );

                offset += size;
                ids.push_back(lastNodesCount);
                lastNodesCount += nodesCount;
            }
        }
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, worldBuffer);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, matrixBuffer);

    glBufferData(GL_SHADER_STORAGE_BUFFER, ids.size(), ids.data(), GL_STATIC_DRAW);

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

    auto octreePosition = glm::ivec3(startPosition / octreeSize);
    octreePosition += glm::min(glm::vec3(0), glm::sign(startPosition));

    // std::cout << "SP x=" << startPosition.x << " y=" << startPosition.y << " z=" << startPosition.z << std::endl;
    // std::cout << "SP / OS x=" << startPositionDivideOctreePosition.x << " y=" << startPositionDivideOctreePosition.y << " z=" << startPositionDivideOctreePosition.z << std::endl;
    // std::cout << "OP x=" << octreePosition.x << " y=" << octreePosition.y << " z=" << octreePosition.z << std::endl;
    // std::cout << "OS " << octreeSize << std::endl;

    const float halfOctreeSize = octreeSize / 2;
    auto rayLength = -(glm::sign(rayDirection) * (glm::mod(startPosition, octreeSize) - halfOctreeSize) - halfOctreeSize) * rayStepSizeSingle;

    glm::bvec3 mask;

    float distance = 0.0f;
    while (distance < maxDistance) {
        if (const std::shared_ptr<Octree> octree = getOctree(octreePosition)) {
            return octree->voxelRaycastTraversal(rayDirection, glm::abs(startPosition + rayDirection * distance));
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
        octreePosition += glm::ivec3(glm::vec3(mask)) * step;

        traceStack.entryStack.push_back(startPosition + rayDirection * distance);
    }

    return traceStack;
}

void OctreeSpace::updateOctrees() {
    dataSize = 0;

    for (int z = 0; z < diameter; z++) {
        const int realZ = spaceCenter.z + z;

        for (int y = 0; y < diameter; y++) {
            const int realY = spaceCenter.y + y;

            for (int x = 0; x < diameter; x++) {
                const int realX = spaceCenter.x + x;

                auto octree = std::make_shared<Octree>(octreeSideSize, maxDepth);
                if (y == 0) {
                    generateOctree(glm::ivec3(realX, radius / 2, realZ), octree);
                }
                // std::cout << "GENERATED " << x << " " << y << " " << z << std::endl;
                // std::cout << "@ " << x + (y * diameter + z) * diameter << std::endl; // 2 + (8) * 3
                octrees[x + (z * diameter + y) * diameter] = octree;

                dataSize += octree->nodesCount() * sizeof(Node);
            }
        }
    }
}

void OctreeSpace::generateOctree(const glm::ivec3& position, const std::shared_ptr<Octree>& octree) const {
    //std::random_device dev;
    //std::mt19937 rng(dev());
    //std::uniform_int_distribution<int> rand(0, 100);

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

    std::cout << "Generation end, nodes count: " << octree->nodesCount() << std::endl;
}
