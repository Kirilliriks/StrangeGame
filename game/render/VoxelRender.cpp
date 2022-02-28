//
// Created by KirilliriK on 28.02.2022.
//

#include "VoxelRender.h"

VoxelRender::VoxelRender() {
    width = 320;
    height = 200;
    shader = Shader(R"(D:\StrangeGame\game\resources\shaders\vertex.glsl)", R"(D:\StrangeGame\game\resources\shaders\fragment.glsl)");

    // Create window object(Quad)
    glGenVertexArrays(1, &windowArrayID);
    glBindVertexArray(windowArrayID);

    glGenBuffers(1, &windowBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, windowBuffer);
    float data[] = {
            -1.0f, -1.0f,
            -1.0f,  1.0f,
            1.0f, -1.0f,
            1.0f,  1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STATIC_DRAW);

    GLint posPtr = shader.getAttribLocation("pos");
    glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(posPtr);
}

void VoxelRender::render(double deltaTime) {
    shader.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    shader.unbind();
}
