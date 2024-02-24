//
// Created by KirilliriK on 17.02.2024.
//

#include "PolygonRender.hpp"

#include <algorithm>

#include "../../Game.hpp"
#include "mesh/MeshBuilder.hpp"
#include "mesh/MeshStorage.hpp"

static int nodeIndex[] = {0};
static bool showPathNode[] = {false};

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

    const glm::vec4 color = showPathNode[0] ? glm::vec4(1.0f, 0.0f, 0.0f, 0.5f) : glm::vec4(0.0f, 1.0f, 1.0f, 0.5f);
    const std::vector<Node> nodes = showPathNode[0] ? lastTrace.pathNodesStack : lastTrace.nodesStack;
    if (nodeIndex[0] > 0 && nodeIndex[0] >= nodes.size()) {
        nodeIndex[0] = nodes.size() - 1;
    }

    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(0), glm::vec4(1.0f, 20 / 25.0f, 1.0f, 0.8f), 0.1f);

    auto mesh = new Mesh(meshBuilder);
    MeshStorage::pushMesh("point_mesh", mesh);

    for (const glm::vec3& point : traceStack.entryStack) {
        objects.emplace_back(point, mesh);
    }

    if (!nodes.empty()) {
        const Node& node = nodes[nodeIndex[0]];

        MeshBuilder meshNodeBuilder;
        meshNodeBuilder.cube(glm::vec3(node.halfSize), color, node.halfSize);

        mesh = new Mesh(meshNodeBuilder);
        MeshStorage::pushMesh("node_mesh", mesh);

        objects.emplace_back(glm::vec3(node.position), mesh);
    }

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

                meshBuilder.cube(glm::vec3(x, y, z) + 0.5f, node.color, 0.5f);
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

    const int maxSize =
        static_cast<int>(showPathNode[0] ? lastTrace.pathNodesStack.size() : lastTrace.nodesStack.size());
    if (Input::q.pressed) {
        tryChangeNodeIndex(maxSize, nodeIndex[0] - 1);
        traceLine(lastTrace);
    }

    if (Input::e.pressed) {
        tryChangeNodeIndex(maxSize, nodeIndex[0] + 1);
        traceLine(lastTrace);
    }

    if (Input::f.pressed) {
        showPathNode[0] = !showPathNode[0];
        traceLine(lastTrace);
    }
}

void PolygonRender::imgui() {
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);

    const int maxSize =
        static_cast<int>(showPathNode[0] ? lastTrace.pathNodesStack.size() : lastTrace.nodesStack.size()) - 1;
    if (ImGui::SliderInt("Node Index", nodeIndex, 0, maxSize)) {
        traceLine(lastTrace);
    }
    if (ImGui::Checkbox("Show Path Nodes", showPathNode)) {
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
