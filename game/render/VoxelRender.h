//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_VOXELRENDER_H
#define STRANGEGAME_VOXELRENDER_H


#include "Shader.h"

class VoxelRender {
public:
    VoxelRender();
    void render(double deltaTime);
private:
    int width, height;
    Shader shader;

    GLuint windowArrayID;
    GLuint windowBuffer;
};


#endif //STRANGEGAME_VOXELRENDER_H
