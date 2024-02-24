//
// Created by KirilliriK on 17.02.2024.
//

#include "PolygonRender.hpp"

#include <algorithm>

#include "../../Game.hpp"
#include "mesh/MeshBuilder.hpp"
#include "mesh/MeshStorage.hpp"

static int nodeIndex[] = {0};

void tryChangeNodeIndex(const int& stackSize, const int& newIndex) {
    if (newIndex < 0 || newIndex >= stackSize) {
        return;
    }

    nodeIndex[0] = newIndex;
}

PolygonRender::PolygonRender(Game* game) : game(*game) {
    shader = Shader(R"(..\game\resources\shaders\polygon\vertex_pol.glsl)",
                    R"(..\game\resources\shaders\polygon\fragment_pol.glsl)");
}

void PolygonRender::traceLine(const TraceStack& traceStack) {
    clearObjects();

    if (nodeIndex[0] > 0 && nodeIndex[0] >= traceStack.nodesStack.size()) {
        nodeIndex[0] = traceStack.nodesStack.size() - 1;
    }

    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(0), glm::vec4(1.0f, 20 / 25.0f, 1.0f, 0.8f), 0.1f);

    auto mesh = new Mesh(meshBuilder);
    MeshStorage::pushMesh("point_mesh", mesh);

    for (const glm::vec3& point : traceStack.entryStack) {
        objects.emplace_back(point, mesh);
    }

    const Node& node = traceStack.nodesStack[nodeIndex[0]];

    MeshBuilder meshNodeBuilder;
    meshNodeBuilder.cube(glm::vec3(node.halfSize - 0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 0.5f), node.halfSize);

    mesh = new Mesh(meshNodeBuilder);
    MeshStorage::pushMesh("node_mesh", mesh);

    objects.emplace_back(glm::vec3(node.position), mesh);

    lastTrace = traceStack;
}

void PolygonRender::rebuildWorld() {
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

    auto object = Object(glm::vec3(0), mesh);
    world = object;
    objects.emplace_back(object);
}

void PolygonRender::update() {
    // objects.erase(
    //     std::ranges::remove_if(objects,
    //                            [](const Object& object) {
    //                                return object.isNeedRemove();
    //                            }
    //     ).begin()
    // );

    if (Input::q.pressed) {
        tryChangeNodeIndex(lastTrace.nodesStack.size(), nodeIndex[0] - 1);
        traceLine(lastTrace);
    }

    if (Input::e.pressed) {
        tryChangeNodeIndex(lastTrace.nodesStack.size(), nodeIndex[0] + 1);
        traceLine(lastTrace);
    }
}

void PolygonRender::imgui() {
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);
    if (ImGui::SliderInt("Node Index", nodeIndex, 0, lastTrace.nodesStack.size() - 1)) {
        traceLine(lastTrace);
    }
    ImGui::End();
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
