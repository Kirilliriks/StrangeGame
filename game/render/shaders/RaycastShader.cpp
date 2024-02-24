//
// Created by KirilliriK on 01.03.2022.
//

#include "RaycastShader.hpp"
#include "Shader.hpp"

RaycastShader::RaycastShader(const std::string& shader) {
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxGroupX);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxGroupY);

    // Creating the compute shader, and the program object containing the shader
    progHandle = glCreateProgram();
    const GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

    const std::string shader_source = Shader::loadShaderFromFile(shader);
    const auto buffer = shader_source.c_str();
    glShaderSource(cs, 1, &buffer, nullptr);

    glCompileShader(cs);
    int rvalue;
    glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in compiling the compute shader\n");
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(cs, 10239, &length, log);
        fprintf(stderr, "Compiler log:\n%s\n", log);
        return;
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
        return;
    }
    glUseProgram(progHandle);
}

GLuint RaycastShader::getHandle() const {
    return progHandle;
}
