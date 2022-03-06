//
// Created by KirilliriK on 28.02.2022.
//

#include "VoxelRender.h"

#include <random>
#include "glm/gtc/noise.hpp"
#include "shaders/RaycastShader.h"
#include "imgui.h"

VoxelRender::VoxelRender(Game *game) : camera(game->getCamera()) {
    this->game = game;
    window = game->getWindow();

    worldSize = octree.getSize();
    shader = Shader(R"(D:\StrangeGame\game\resources\shaders\vertex.glsl)",
                    R"(D:\StrangeGame\game\resources\shaders\fragment.glsl)");

    // Create window object(Quad)
    glGenVertexArrays(1, &windowArrayID);
    glBindVertexArray(windowArrayID);

    glGenBuffers(1, &windowBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, windowBuffer);
    float data[] = {
            -1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STATIC_DRAW);

    GLint posPtr = shader.getAttribLocation("pos");
    glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(posPtr);

    genTexture(); // WINDOW TEXTURE

    rayShader = new RaycastShader(R"(D:\StrangeGame\game\resources\shaders\newnewcaster.comp)");
    raycastShaderID = rayShader->getHandle();
}

void VoxelRender::update(double deltaTime) {
    debugCast = octree.castNode(camera.getDirection(), camera.getPosition());
    frontVoxel = debugCast.voxelPos;
    correctVoxel = octree.voxelRaycast(camera.getDirection(), camera.getPosition(), 500.0f);
}

void VoxelRender::render(double deltaTime) {
    glUseProgram(raycastShaderID);

    glUniform3f(2, camera.getX(), camera.getY(), camera.getZ());
    glUniform2f(3, camera.getYaw(), camera.getPitch());
    glUniform2f(4, (float)window->width, (float)window->height);

    glUniform3i(5, frontVoxel.x, frontVoxel.y, frontVoxel.z);
    glUniform3i(6, debugCast.nodePos.x, debugCast.nodePos.y, debugCast.nodePos.z);
    glUniform1i(7, debugCast.nodeSize);
    glUniform3i(8, debugCast.lastStepPos.x, debugCast.lastStepPos.y, debugCast.lastStepPos.z);
    glUniform3i(9, debugCast.preLastStepPos.x, debugCast.preLastStepPos.y, debugCast.preLastStepPos.z);
    glUniform3i(10, correctVoxel.x, correctVoxel.y, correctVoxel.z);

    glDispatchCompute((GLuint)(window->width), (GLuint)(window->height), 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    shader.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    shader.unbind();
}

void VoxelRender::imgui(double deltaTime) {
    static glm::ivec3 lastPos = glm::ivec3(0, 0, 0);
    /// TODO
    ImGui::Begin("Info window");
    ImGui::SetWindowSize(ImVec2(300, 300));
    ImGui::Text("Cam x=%d y=%d z=%d", (int)camera.getX(), (int)camera.getY(), (int)camera.getZ());
    ImGui::Text("VoxelTry x=%d y=%d z=%d", frontVoxel.x, frontVoxel.y, frontVoxel.z);
    ImGui::Text("VoxelCorrect x=%d y=%d z=%d", correctVoxel.x, correctVoxel.y, correctVoxel.z);
    ImGui::Text("VoxelFloat x=%f y=%f z=%f", debugCast.voxelFloatPos.x, debugCast.voxelFloatPos.y, debugCast.voxelFloatPos.z);
    if (frontVoxel.x != -1 || frontVoxel.y != -1 || frontVoxel.z != -1) {
        lastPos = frontVoxel;
    }
    ImGui::Text("LastStepPos x=%d y=%d z=%d", debugCast.lastStepPos.x, debugCast.lastStepPos.y, debugCast.lastStepPos.z);
    ImGui::Text("PreLastStepPos x=%d y=%d z=%d", debugCast.preLastStepPos.x, debugCast.preLastStepPos.y, debugCast.preLastStepPos.z);
    ImGui::Text("NodePos x=%d y=%d z=%d", debugCast.nodePos.x, debugCast.nodePos.y, debugCast.nodePos.z);
    ImGui::Text("SubVector x=%d y=%d z=%d", debugCast.subVector.x, debugCast.subVector.y, debugCast.subVector.z);
    ImGui::Text("Distance = %f", debugCast.distance);
    ImGui::Text("NodeSize = %d", debugCast.nodeSize);
    ImGui::Text("Depth = %d", debugCast.depth);
    ImGui::Text("PassedNodes = %d", debugCast.passedNodes);
    ImGui::Text("Step x=%d y=%d z=%d", debugCast.step.x, debugCast.step.y, debugCast.step.z);
    ImGui::End();
    ImGui::Render();
    ///w
}

void VoxelRender::createWorld() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand(-1024,1024);
    int seed = rand(rng);
    for (int z = 0; z < worldSize; z++) {
        for (int x = 0; x < worldSize; x++) {
            float per = glm::simplex(glm::vec3(x / 32.0f, z / 32.0f, 21));
            per = (per + 1) / 2;
            int y = (int)(per * (float)32);
            if (y < 0 || y > worldSize) continue;

            for (int i = 0; i <= y; i++) {
                octree.setVoxel(glm::ivec3(x, i, z), glm::vec4(per, per, per, 1.0f));
            }
        }
    }

    updateWorld();

    std::cout << "Generation end, nodes count: " << octree.nodesCount() << std::endl;
}

void VoxelRender::updateWorld() {
    glGenBuffers(1, &worldBufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, worldBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 octree.nodesCount() * sizeof(Node),
                 octree.getData(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, worldBufferID);
}

// Texture for display window
GLuint VoxelRender::genTexture() {
    GLuint texHandle;
    glGenTextures(1, &texHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window->width, window->height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    return texHandle;
}
