//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_SHADER_H
#define STRANGEGAME_SHADER_H

#include <iostream>
#include "glm/glm.hpp"

#include "gl.h"

class Shader {
public:
    static std::string loadShaderFromFile(const std::string& path);
    static GLint createShader(const std::string& shader, GLuint shaderPtr);

    Shader();
    Shader(const std::string& vertexName, const std::string& fragmentName);

    GLint getUniformLocation(const std::string& name) const;
    GLint getAttribLocation(const std::string& name) const;

    void uniformMatrix(const std::string& name, glm::mat4 matrix) const;

    void bind() const;
    void unbind() const;

    GLint initializeShader(const std::string& vertex, const std::string& fragment);
private:
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderID;
};



#endif //STRANGEGAME_SHADER_H
