//
// Created by KirilliriK on 20.04.2024.
//

#ifndef OCTREESPACE_HPP
#define OCTREESPACE_HPP
#include <memory>
#include <vector>

#include "Octree.hpp"

class OctreeSpace {
public:

    explicit OctreeSpace(const int& radius);
    ~OctreeSpace() = default;

    void updateSpaceCenter(const glm::ivec3& position);
    void setVoxel(const glm::ivec3& position, const glm::vec4& color);
    void setOctree(const glm::ivec3& position, std::shared_ptr<Octree> octree);
    auto getOctree(const glm::ivec3& position, const bool& local = false) -> std::shared_ptr<Octree>;
    glm::ivec3 getOctreePosition(const glm::ivec3& position) const;

    int getOctreeSideSize() const;
    int getRadius() const;
    int getDiameter() const;

    glm::ivec3 getSpaceCenter() const;

    void fillBuffers(const int& worldBuffer, const int& matrixBuffer);

    TraceStack voxelRaycast(const glm::vec3& rayDirection, const glm::vec3& startPosition, float maxDistance);
private:
    void updateOctrees();
    void generateOctree(const glm::ivec3& position, const std::shared_ptr<Octree>& octree) const;

    const int maxDepth = 8;
    const int octreeSideSize = 1 << maxDepth; // std::pow(2, maxDepth);

    int radius;
    int diameter;
    int dataSize;
    std::vector<std::shared_ptr<Octree>> octrees;

    glm::ivec3 spaceCenter = glm::ivec3(-1, -1, -1);
};


#endif //OCTREESPACE_HPP
