//
// Created by KirilliriK on 17.02.2024.
//

#include "PolygonRender.hpp"

#include <algorithm>

#include "../../Game.hpp"
#include "mesh/MeshBuilder.hpp"
#include "mesh/MeshStorage.hpp"

PolygonRender::PolygonRender(Game* game) : game(*game) {
    shader = Shader(R"(..\game\resources\shaders\polygon\vertex_pol.glsl)",
                    R"(..\game\resources\shaders\polygon\fragment_pol.glsl)");
}

void PolygonRender::traceLine(const std::vector<glm::vec3>& points) {
    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(0, 0, 0), glm::vec4(1.0f, 20 / 25.0f, 1.0f, 1), 0.1f);
    const auto mesh = new Mesh(meshBuilder);
    MeshStorage::pushMesh("point_mesh", mesh);

    clearObjects();
    for (const glm::vec3& point : points) {
        objects.emplace_back(point, mesh);
    }
}

void PolygonRender::updateWorld() {
    MeshBuilder meshBuilder;

    Octree& octree = game.getWorld()->getOctree();
    const int halfSize = octree.getSize();

    for (int y = 0; y < halfSize; y++) {
        for (int z = 0; z < halfSize; z++) {
            for (int x = 0; x < halfSize; x++) {
                Node node = octree.getVoxel(glm::ivec3(x, y, z));
                if (node.position.x <= -1 || node.color.a <= 0.0f) {
                    continue;
                }

                meshBuilder.cube(glm::vec3(x, y, z), node.color, 0.5f);
            }
        }
    }

    const auto mesh = new Mesh(meshBuilder);
    MeshStorage::pushMesh("world_mesh", mesh);

    auto object = Object(glm::vec3(0, 0, 0), mesh);
    world = object;
    objects.emplace_back(object);
}

void PolygonRender::update() {
    objects.erase(
        std::ranges::remove_if(objects,
                               [](const Object& object) {
                                   return object.isNeedRemove();
                               }
        ).begin()
    );
}


void PolygonRender::render(const double& deltaTime) const {
    shader.bind();
    shader.uniformMatrix("projection_view", game.getWorld()->getCamera().getMatrixMultiply());

    for (const Object& object: objects) {
        shader.uniformMatrix("model", object.getModelMatrix());
        object.render();
    }

    shader.unbind();
}

void PolygonRender::clearObjects() {
    objects.clear();
    objects.emplace_back(world);
}
