//
// Created by KirilliriK on 28.02.2022.
//

#include "Shader.hpp"

#include <sstream>
#include <fstream>

Shader::Shader() {
    initializeShader(defaultVertexShader, defaultFragmentShader);
}

Shader::Shader(const std::string &vertexName, const std::string &fragmentName) {
    const std::string vertex = loadShaderFromFile(vertexName);
    const std::string fragment = loadShaderFromFile(fragmentName);
    initializeShader(vertex, fragment);
}

GLint Shader::getUniformLocation(const std::string &name) const {
    bind();
    return glGetUniformLocation(shaderID, name.c_str());
}

GLint Shader::getAttribLocation(const std::string &name) const {
    bind();
    return glGetAttribLocation(shaderID, name.c_str());
}

void Shader::bind() const {
    glUseProgram(shaderID);
}

void Shader::unbind() const {
    glUseProgram(0);
}

GLint Shader::initializeShader(const std::string &vertex, const std::string &fragment) {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Trying to create shaders
    if (!vertexShader || !fragmentShader) {
        return GL_FALSE;
    }

    // Trying to compile vertex shader
    std::cout << "Compiling vertex shader..." << std::endl;
    GLint vertex_result = createShader(vertex, vertexShader);
    if (vertex_result != GL_TRUE) {
        return vertex_result;
    }

    // Trying to compile fragment shader
    std::cout << "Compiling fragment shader..." << std::endl;
    GLint fragment_result = createShader(fragment, fragmentShader);
    if (fragment_result != GL_TRUE) {
        return fragment_result;
    }

    // Link the vertex and fragment shader into a program
    if (!(shaderID = glCreateProgram())) {
        return GL_FALSE;
    } else {
        GLint status;

        glAttachShader(shaderID, vertexShader);
        glAttachShader(shaderID, fragmentShader);
        glLinkProgram(shaderID);
        glGetProgramiv(shaderID, GL_LINK_STATUS, &status);

        // Retrieve compilation error
        if (GL_TRUE != status) {
            GLint infoLogLength;
            char *strInfoLog;

            glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
            strInfoLog = new char[infoLogLength + 1];
            glGetProgramInfoLog(shaderID, infoLogLength, nullptr, strInfoLog);
            std::cout << strInfoLog << std::endl;
            delete[] strInfoLog;

            return GL_FALSE;
        }
    }

    return GL_TRUE;
}

std::string Shader::loadShaderFromFile(const std::string &path) {
    std::stringstream shader;

    std::ifstream fileIn;

    fileIn.open(path, std::ifstream::in);

    std::string line;
    if (fileIn) {
        while (std::getline(fileIn, line)) {
            shader << line + '\n';
        }
    } else {
        std::cout << "Error, '" << path << "' not found." << std::endl;
    }

    return shader.str();
}

GLint Shader::createShader(const std::string &shader, GLuint shaderPtr) {
    const auto buffers = static_cast<const GLchar *>(shader.c_str());
    auto shaderCodeSize = GLint(shader.size());
    GLint compileStatus;

    glShaderSource(shaderPtr, 1, &buffers, &shaderCodeSize);
    glCompileShader(shaderPtr);
    glGetShaderiv(shaderPtr, GL_COMPILE_STATUS, &compileStatus);
    if (GL_TRUE != compileStatus) {
        GLint infoLogLength;
        char *strInfoLog;

        glGetShaderiv(shaderPtr, GL_INFO_LOG_LENGTH, &infoLogLength);
        strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shaderPtr, infoLogLength, nullptr, strInfoLog);

        std::cout << strInfoLog << std::endl;

        delete[] strInfoLog;

        return compileStatus;
    }

    return GL_TRUE;
}

const char* Shader::defaultVertexShader =
"#version 430\n in vec2 pos; out vec2 texCoord; void main() { texCoord = pos*0.5f + 0.5f; gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); }";

const char* Shader::defaultFragmentShader =
"#version 430\n uniform sampler2D srcTex; in vec2 texCoord; out vec4 color; void main() { vec3 c = texture(srcTex, texCoord).xyz; color = vec4(1, 0, 0, 1.0); }";
