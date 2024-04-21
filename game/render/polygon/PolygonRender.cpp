//
// Created by KirilliriK on 17.02.2024.
//

#include "PolygonRender.hpp"

#include "../../Game.hpp"
#include "mesh/MeshBuilder.hpp"
#include "mesh/MeshStorage.hpp"

static int nodeIndex[] = {};
static bool showPathNode[] = {false};
static bool showTestTrace[] = {false};

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

void PolygonRender::traceLine(const TraceStack& testStack, const TraceStack& traceStack) {
    clearObjects();

    lastTrace = traceStack;
    testTrace = testStack;

    const TraceStack& trace = showTestTrace[0] ? testTrace : lastTrace;

    const glm::vec4 color = showPathNode[0] ? glm::vec4(1.0f, 0.0f, 0.0f, 0.5f) : glm::vec4(0.0f, 1.0f, 1.0f, 0.5f);
    const std::vector<Node> nodes = showPathNode[0] ? trace.pathNodesStack : trace.nodesStack;
    if (nodeIndex[0] > 0 && nodeIndex[0] >= nodes.size()) {
        nodeIndex[0] = nodes.size() - 1;
    }

    MeshBuilder meshBuilder;
    meshBuilder.cube(glm::vec3(0), glm::vec4(1.0f, 20 / 25.0f, 1.0f, 0.8f), 0.1f);

    auto mesh = new Mesh(meshBuilder);
    MeshStorage::pushMesh("point_mesh", mesh);

    for (const glm::vec3& point : trace.entryStack) {
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

    MeshBuilder meshVoxelBuilder;
    meshVoxelBuilder.cube(glm::vec3(0.51f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.51f);

    mesh = new Mesh(meshVoxelBuilder);
    MeshStorage::pushMesh("target_voxel_mesh", mesh);

    objects.emplace_back(glm::vec3(trace.voxelPos), mesh);
}

void PolygonRender::rebuildWorld() {
    world.clear();

    OctreeSpace octreeSpace = game.getWorld()->getOctreeSpace();
    const int octreeSize = octreeSpace.getOctreeSideSize();
    const int radius = octreeSpace.getRadius();
    for (int zOctree = -radius; zOctree <= radius; zOctree++) {
        for (int yOctree = -radius; yOctree <= radius; yOctree++) {
            for (int xOctree = -radius; xOctree <= radius; xOctree++) {

                auto octree = octreeSpace.getOctree(glm::ivec3(xOctree, yOctree, zOctree), true);
                if (octree->nodesCount() <= 1) {
                    continue;
                }

                MeshBuilder meshBuilder;

                for (int z = 0; z < octreeSize; z++) {
                    for (int y = 0; y < octreeSize; y++) {
                        for (int x = 0; x < octreeSize; x++) {
                            Node node = octree->getVoxel(glm::ivec3(x, y, z));
                            if (node.position.x <= -1 || node.color.a <= 0.0f) {
                                continue;
                            }

                            meshBuilder.cube(glm::vec3(x, y, z) + 0.5f, node.color, 0.5f);
                        }
                    }
                }

                const auto mesh = new Mesh(meshBuilder);
                MeshStorage::pushMesh(
                    "world_mesh-" + std::to_string(xOctree) +
                    "-" + std::to_string(yOctree) +
                    "-" + std::to_string(zOctree),
                    mesh
                );

                auto object = Object(glm::vec3(xOctree * octreeSize, yOctree * octreeSize, zOctree * octreeSize), mesh);
                world.emplace_back(object);
                objects.emplace_back(object);
            }
        }
    }
}

void PolygonRender::update() {
    // objects.erase(
    //     std::ranges::remove_if(objects,
    //                            [](const Object& object) {
    //                                return object.isNeedRemove();
    //                            }
    //     ).begin()
    // );

    const TraceStack& trace = showTestTrace[0] ? testTrace : lastTrace;
    const int maxSize =
        static_cast<int>(showPathNode[0] ? trace.pathNodesStack.size() : trace.nodesStack.size());
    if (Input::Q.pressed) {
        tryChangeNodeIndex(maxSize, nodeIndex[0] - 1);
        traceLine(testTrace, lastTrace);
    }

    if (Input::E.pressed) {
        tryChangeNodeIndex(maxSize, nodeIndex[0] + 1);
        traceLine(testTrace, lastTrace);
    }

    if (Input::T.pressed) {
        showPathNode[0] = !showPathNode[0];
        traceLine(testTrace, lastTrace);
    }

    if (Input::F.pressed) {
        showTestTrace[0] = !showTestTrace[0];
        traceLine(testTrace, lastTrace);
    }
}

void PolygonRender::imgui() {
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);

    const TraceStack& trace = showTestTrace[0] ? testTrace : lastTrace;

    ImGui::Text("Selected voxel x=%d y=%d z=%d",
        trace.voxelPos.x,
        trace.voxelPos.y,
        trace.voxelPos.z
    );

    const int maxSize =
        static_cast<int>(showPathNode[0] ? trace.pathNodesStack.size() : trace.nodesStack.size()) - 1;
    if (ImGui::SliderInt("Node Index", nodeIndex, 0, maxSize)) {
        traceLine(testTrace, lastTrace);
    }
    if (ImGui::Checkbox("Show Path Nodes", showPathNode)) {
        traceLine(testTrace, lastTrace);
    }
    if (ImGui::Checkbox("Show Test Trace", showTestTrace)) {
        traceLine(testTrace, lastTrace);
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
    for (const auto& object : world) {
        objects.emplace_back(object);
    }
}
