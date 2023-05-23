//
// Created by KirilliriK on 28.02.2022.
//

#include "VoxelRender.h"

#include "shaders/RaycastShader.h"
#include "imgui.h"

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

void VoxelRender::render(double deltaTime) {
    glUseProgram(raycastShaderID);

    Camera camera = world->getCamera();
    Octree::DebugCast debugCast = world->getDebugCast();

    glUniform3f(2, camera.getX(), camera.getY(), camera.getZ());
    glUniform2f(3, camera.getYaw(), camera.getPitch());
    glUniform2f(4, window->width, window->height);
    glUniform3i(5, debugCast.voxelPos.x, debugCast.voxelPos.y, debugCast.voxelPos.z);
    glUniform3i(6, debugCast.voxelPos.x, debugCast.voxelPos.y, debugCast.voxelPos.z);

    glDispatchCompute(window->width / groupSize, window->height / groupSize, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    shader.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    shader.unbind();
}

void VoxelRender::updateWorld() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, worldBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 world->getOctree().nodesCount() * sizeof(Node),
                 world->getOctree().getData(), GL_STATIC_DRAW);
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
