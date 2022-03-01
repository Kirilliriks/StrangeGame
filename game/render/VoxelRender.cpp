//
// Created by KirilliriK on 28.02.2022.
//

#include "VoxelRender.h"

#include <random>
#include "glm/gtc/noise.hpp"

GLuint createComputeShader(const std::string &shader, bool from_file);

VoxelRender::VoxelRender(Game *game) : camera(game->getCamera()) {
    this->game = game;
    window = game->getWindow();

    worldSize = 400;
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
    raycastShaderID = createComputeShader(R"(D:\StrangeGame\game\resources\shaders\newcaster.comp)", true);
}

void VoxelRender::render(double deltaTime) {
    glUseProgram(raycastShaderID);

    glUniform3f(2, camera.getX(), camera.getY(), camera.getZ());
    glUniform2f(3, camera.getYaw(), camera.getPitch());
    glUniform2f(4, (float)window->width, (float)window->height);

    glDispatchCompute((GLuint)window->width, (GLuint)window->height, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);
    shader.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    shader.unbind();
}

void VoxelRender::createWorld() {
    int doubleSize = worldSize * worldSize;
    worldBuffer.resize(worldSize * doubleSize);


    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand(-1024,1024);
    int seed = rand(rng);
    for (int z = 0; z < worldSize; z++) {
        for (int x = 0; x < worldSize; x++) {
            float per = glm::simplex(glm::vec3(x / 32.0f, z / 32.0f, seed));
            per = (per + 1) / 2;
            //std::cout << "Per " << per << std::endl;
            int y = (int)(per * (float)32);
            if (y < 0 || y >= worldSize) continue;


            for (int i = 0; i <= y; i++) {
                worldBuffer[x + i * worldSize + z * doubleSize] = Color(per, per, 1);
            }
        }
    }

    updateWorld();
}

void VoxelRender::updateWorld() {
    glGenBuffers(1, &worldBufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, worldBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, worldBuffer.size() * sizeof(Color), worldBuffer.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, worldBufferID);
}

// Texture for display window
GLuint VoxelRender::genTexture() {
    std::cout << "GenTexture" << std::endl;

    GLuint texHandle;
    glGenTextures(1, &texHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window->width, window->height, 0, GL_RGBA, GL_FLOAT, nullptr);

    // Because we're also using this tex as an image (in order to write to it),
    // we bind it to an image unit as well
    glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    std::cout << "DONE" << std::endl;
    return texHandle;
}

//TODO
GLuint createComputeShader(const std::string &shader, bool from_file) {
    // Creating the compute shader, and the program object containing the shader
    GLuint progHandle = glCreateProgram();
    GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

    // In order to write to a texture, we have to introduce it as image2D.
    // local_size_x/y/z layout variables define the work group size.
    // gl_GlobalInvocationID is a uvec3 variable giving the global ID of the thread,
    // gl_LocalInvocationID is the local index within the work group, and
    // gl_WorkGroupID is the work group's index

    if (from_file) {
        std::string shader_source = Shader::loadShaderFromFile(shader);
        auto buffer = static_cast<const GLchar *>(shader_source.c_str());
        glShaderSource(cs, 1, &buffer, nullptr);
    } else {
        auto buffer = static_cast<const GLchar *>(shader.c_str());
        glShaderSource(cs, 1, &buffer, nullptr);
    }

    glCompileShader(cs);
    int rvalue;
    glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in compiling the compute shader\n");
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(cs, 10239, &length, log);
        fprintf(stderr, "Compiler log:\n%s\n", log);

        return 0;
    }
    glAttachShader(progHandle, cs);

    glLinkProgram(progHandle);
    glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in linking compute shader program\n");
        GLchar log[10240];
        GLsizei length;
        glGetProgramInfoLog(progHandle, 10239, &length, log);
        fprintf(stderr, "Linker log:\n%s\n", log);

        return 0;
    }
    glUseProgram(progHandle);

    return progHandle;
}
