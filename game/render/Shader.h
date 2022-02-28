//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_SHADER_H
#define STRANGEGAME_SHADER_H

#include <iostream>
#include <glad.h>

class Shader {
public:
    Shader();
    Shader(const std::string& vertexName, const std::string& fragmentName);

    GLint getUniformLocation(const std::string& uniform_name) const;
    GLint getAttribLocation(const std::string& attrib_name) const;

    void bind() const;
    void unbind() const;

    GLint initializeShader(const std::string& vertex, const std::string& fragment);

    static const char* defaultVertexShader;
    static const char* defaultFragmentShader;
protected:
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderID;
private:
    static std::string loadShaderFromFile(const std::string& path);
    static GLint createShader(const std::string& shader, GLuint shaderPtr);
};



#endif //STRANGEGAME_SHADER_H
