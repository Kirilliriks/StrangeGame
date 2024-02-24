//
// Created by KirilliriK on 28.02.2022.
//

#include "Shader.hpp"

#include <sstream>
#include <fstream>

#include "glm/gtc/type_ptr.hpp"

std::string Shader::loadShaderFromFile(const std::string &path) {
    std::stringstream shader;

    std::ifstream fileIn;

    fileIn.open(path, std::ifstream::in);

    if (fileIn) {
        std::string line;
        while (std::getline(fileIn, line)) {
            shader << line + '\n';
        }
    } else {
        std::cout << "Error, '" << path << "' not found." << std::endl;
    }

    return shader.str();
}

GLint Shader::createShader(const std::string &shader, GLuint shaderPtr) {
    const auto buffers = shader.c_str();
    const auto shaderCodeSize = static_cast<GLint>(shader.size());
    GLint compileStatus;

    glShaderSource(shaderPtr, 1, &buffers, &shaderCodeSize);
    glCompileShader(shaderPtr);
    glGetShaderiv(shaderPtr, GL_COMPILE_STATUS, &compileStatus);
    if (GL_TRUE != compileStatus) {
        GLint infoLogLength;

        glGetShaderiv(shaderPtr, GL_INFO_LOG_LENGTH, &infoLogLength);
        const auto strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shaderPtr, infoLogLength, nullptr, strInfoLog);

        std::cout << strInfoLog << std::endl;

        delete[] strInfoLog;

        return compileStatus;
    }

    return GL_TRUE;
}

Shader::Shader(): vertexShader(-1), fragmentShader(-1), shaderID(-1) {
}

Shader::Shader(const std::string &vertexName, const std::string &fragmentName) {
    const std::string vertex = loadShaderFromFile(vertexName);
    const std::string fragment = loadShaderFromFile(fragmentName);
    initializeShader(vertex, fragment);
}

GLint Shader::getUniformLocation(const std::string& name) const {
    bind();
    return glGetUniformLocation(shaderID, name.c_str());
}

GLint Shader::getAttribLocation(const std::string& name) const {
    bind();
    return glGetAttribLocation(shaderID, name.c_str());
}

void Shader::uniformMatrix(const std::string& name, glm::mat4 matrix) const {
    const GLint transformLoc = glGetUniformLocation(shaderID, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
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
    if (const GLint vertex_result = createShader(vertex, vertexShader); vertex_result != GL_TRUE) {
        return vertex_result;
    }

    // Trying to compile fragment shader
    std::cout << "Compiling fragment shader..." << std::endl;
    if (const GLint fragment_result = createShader(fragment, fragmentShader); fragment_result != GL_TRUE) {
        return fragment_result;
    }

    // Link the vertex and fragment shader into a program
    if (!(shaderID = glCreateProgram())) {
        return GL_FALSE;
    }

    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glLinkProgram(shaderID);

    GLint status;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &status);

    // Retrieve compilation error
    if (GL_TRUE != status) {
        GLint infoLogLength;

        glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        const auto strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(shaderID, infoLogLength, nullptr, strInfoLog);
        std::cout << strInfoLog << std::endl;
        delete[] strInfoLog;

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return GL_FALSE;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return GL_TRUE;
}