//
// Created by KirilliriK on 28.02.2022.
//
#include <gl.h>

#include "VoxelRender.hpp"
#include "../shaders/RaycastShader.hpp"

VoxelRender::VoxelRender(Game *game) : world(game->getWorld()) {
    this->game = game;
    window = game->getWindow();

    shader = Shader(R"(..\game\resources\shaders\vertex.glsl)",
                    R"(..\game\resources\shaders\fragment.glsl)");

    // Create window object(Quad)
    glGenVertexArrays(1, &windowArrayID);
    glBindVertexArray(windowArrayID);

    glGenBuffers(1, &windowBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, windowBufferID);
    constexpr float data[] = {
            -1.0f, 1.0f,
            -1.0f, -1.0f,
            1.0f, 1.0f,
            1.0f, -1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STATIC_DRAW);

    const GLint posPtr = shader.getAttribLocation("pos");
    glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(posPtr);

    genTexture(); // WINDOW TEXTURE
    glGenBuffers(1, &worldBufferID);
    glGenBuffers(1, &matrixBufferID);

    rayShader = new RaycastShader(R"(..\game\resources\shaders\raytracing\raycaster.comp)");
    raycastShaderID = rayShader->getHandle();
}

void VoxelRender::render(const double& deltaTime) const {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUseProgram(raycastShaderID);

    const Camera& camera = world->getCamera();
    const glm::ivec3& previouseVoxelPos = world->getTraceCast().previousVoxelPos;
    const glm::ivec3& voxelPos = world->getTraceCast().voxelPos;

    glUniform3f(2, camera.getX(), camera.getY(), camera.getZ());
    glUniform2f(3, glm::radians(camera.getYaw()), glm::radians(camera.getPitch()));
    glUniform2f(4, window->width, window->height);
    glUniform3i(5, previouseVoxelPos.x, previouseVoxelPos.y, previouseVoxelPos.z);
    glUniform3i(6, voxelPos.x, voxelPos.y, voxelPos.z);
    glUniform1i(7, world->getOctreeSpace().getRadius());
    glUniform1i(8, world->getOctreeSpace().getDiameter());

    const glm::ivec3 spaceCenter = world->getOctreeSpace().getSpaceCenter();
    glUniform3i(9, spaceCenter.x, spaceCenter.y, spaceCenter.z);

    glDispatchCompute(window->width / groupSize, window->height / groupSize, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    shader.bind();
    glBindVertexArray(windowArrayID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    shader.unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void VoxelRender::updateWorld() const {
    world->getOctreeSpace().fillBuffers(worldBufferID, matrixBufferID);
}

// Texture for display window
void VoxelRender::genTexture() {
    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window->width, window->height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glBindTexture(GL_TEXTURE_2D, 0);
}
