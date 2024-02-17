//
// Created by KirilliriK on 01.03.2022.
//

#ifndef STRANGEGAME_RAYCASTSHADER_H
#define STRANGEGAME_RAYCASTSHADER_H

#include <iostream>
#include "glad.h"

class RaycastShader {
public:
    RaycastShader(std::string shader);
    GLuint getHandle();
    int maxGroupX, maxGroupY;
private:
    GLuint progHandle;
};


#endif //STRANGEGAME_RAYCASTSHADER_H
