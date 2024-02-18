//
// Created by KirilliriK on 17.02.2024.
//

#include "PolygonRender.hpp"

#include "../../Game.hpp"
#include "mesh/MeshBuilder.hpp"
#include "mesh/MeshStorage.hpp"

PolygonRender::PolygonRender(Game* game) : game(*game) {
    shader = Shader(R"(..\game\resources\shaders\polygon\vertex_pol.glsl)",
                        R"(..\game\resources\shaders\polygon\fragment_pol.glsl)");

    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), 2.0f);
    const auto mesh = new Mesh(meshBuilder);
    MeshStorage::pushMesh("test_mesh", mesh);

    auto object = Object(glm::vec3(0, 0, 1), mesh);
    objects.emplace_back(object);
}

void PolygonRender::updateWorld() {

}


void PolygonRender::render(double deltaTime) const {
    shader.bind();
    shader.uniformMatrix("projection_view", game.getWorld()->getCamera().getMatrixMultiply());

    for (const Object& object : objects) {
        shader.uniformMatrix("model", object.getModelMatrix());
        object.render();
    }

    shader.unbind();
}
