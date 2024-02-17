//
// Created by KirilliriK on 17.02.2024.
//

#include "PolygonRender.hpp"

#include "../../Game.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

PolygonRender::PolygonRender(Game* game) : game(*game) {
    shader = Shader(R"(..\game\resources\shaders\polygon\vertex_pol.glsl)",
                        R"(..\game\resources\shaders\polygon\fragment_pol.glsl)");

    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(5, 5, 5), glm::vec3(1, 0, 0), 5);
    objects.emplace_back(meshBuilder);
}

void PolygonRender::updateWorld() {
}

void PolygonRender::render(double deltaTime) const {
    shader.bind();
    shader.uniformMatrix("projection_view", game.getWorld()->getCamera().getMatrixMultiply());

    for (const Mesh& mesh : objects) {
        mesh.render();
    }

    shader.unbind();
}
