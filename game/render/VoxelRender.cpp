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

    shader = Shader(R"(..\game\resources\shaders\vertex.glsl)",
                    R"(..\game\resources\shaders\fragment.glsl)");

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
    glGenBuffers(1, &worldBufferID);

    rayShader = new RaycastShader(R"(..\game\resources\shaders\newnewcaster.comp)");
    raycastShaderID = rayShader->getHandle();
}

void VoxelRender::update(double deltaTime) {
    debugCast = world.getOctree().raycastVoxel(camera.getDirection(), camera.getPosition());
    const glm::ivec3 v = debugCast.voxelPos;//octree.voxelRaycast(camera.getDirection(), camera.getPosition(), 500);
    frontVoxel = glm::vec3(v);

    GLFWwindow * glWindow = window->getGLWindow();
    int state = glfwGetMouseButton(glWindow, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        world.getOctree().setVoxel(v, glm::vec4(255, 0, 0, 255));
        updateWorld();
    }
}

void VoxelRender::render(double deltaTime) {
    glUseProgram(raycastShaderID);

    glUniform3f(2, camera.getX(), camera.getY(), camera.getZ());
    glUniform2f(3, camera.getYaw(), camera.getPitch());
    glUniform2f(4, window->width, window->height);
    glUniform3i(5, debugCast.voxelPos.x, debugCast.voxelPos.y, debugCast.voxelPos.z);
    glUniform3i(6, debugCast.voxelPos.x, debugCast.voxelPos.y, debugCast.voxelPos.z);

    glDispatchCompute(window->width / 8, window->height / 8, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    shader.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    shader.unbind();
}

void VoxelRender::imgui(double deltaTime) {
    static glm::ivec3 lastPos = glm::ivec3(0, 0, 0);
    /// TODO
    ImGui::Begin("Info window");
    ImGui::SetWindowCollapsed(false);
    ImGui::SetWindowSize(ImVec2(200, 150));
    ImGui::Text("Cam x=%d y=%d z=%d", (int)camera.getX(), (int)camera.getY(), (int)camera.getZ());
    ImGui::Text("Voxel x=%d y=%d z=%d", frontVoxel.x, frontVoxel.y, frontVoxel.z);
    ImGui::Text("Try x=%d y=%d z=%d", debugCast.voxelPos.x, debugCast.voxelPos.y, debugCast.voxelPos.z);
    ImGui::Text("Distance %f", debugCast.distance);
    ImGui::End();
    ImGui::Render();
    ///w
}

void VoxelRender::createWorld() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand(0,100);
    const int seed = rand(rng);
    const int worldSize = world.getOctree().getSize();
    for (int z = 0; z < worldSize; z++) {
        for (int x = 0; x < worldSize; x++) {
            float per = glm::simplex(glm::vec3(x / 32.0f, z / 32.0f, 21));
            per = (per + 1) / 2;
            int y = (int)(per * (float)32);
            if (y < 0 || y > worldSize) continue;

            for (int i = 0; i <= y; i++) {
                world.setVoxel(glm::ivec3(x, i, z), glm::vec4(per, per, per, 1.0f));
            }
        }
    }

    updateWorld();

    std::cout << "Generation end, nodes count: " << world.getOctree().nodesCount() << std::endl;
}

void VoxelRender::updateWorld() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, worldBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 world.getOctree().nodesCount() * sizeof(Node),
                 world.getOctree().getData(), GL_STATIC_DRAW);
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
