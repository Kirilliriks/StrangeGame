//
// Created by KirilliriK on 01.03.2022.
//

#include "RaycastShader.h"
#include "Shader.h"

RaycastShader::RaycastShader(std::string shader) {
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxGroupX);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxGroupY);

    // Creating the compute shader, and the program object containing the shader
    progHandle = glCreateProgram();
    GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

    // In order to write to a texture, we have to introduce it as image2D.
    // local_size_x/y/z layout variables define the work group halfSize.
    // gl_GlobalInvocationID is a uvec3 variable giving the global ID of the thread,
    // gl_LocalInvocationID is the local index within the work group, and
    // gl_WorkGroupID is the work group's index

    std::string shader_source = Shader::loadShaderFromFile(shader);
    auto buffer = static_cast<const GLchar *>(shader_source.c_str());
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

GLuint RaycastShader::getHandle() {
    return progHandle;
}
