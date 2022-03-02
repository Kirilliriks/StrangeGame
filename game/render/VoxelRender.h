//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_VOXELRENDER_H
#define STRANGEGAME_VOXELRENDER_H
#define GLFW_INCLUDE_NONE

#include <vector>
#include "GLFW/glfw3.h"
#include "shaders/Shader.h"
#include "../Game.h"
#include "Camera.h"
#include "shaders/RaycastShader.h"
#include "../world/Octree.h"

class Game;
class VoxelRender {
public:
    VoxelRender(Game *game);
    void createWorld();
    void updateWorld();
    void render(double deltaTime);

    GLuint genTexture();
private:
    Game *game;
    Camera &camera;
    Window *window;

    int worldSize;
    GLuint raycastShaderID;
    Shader shader;

    GLuint windowArrayID;
    GLuint windowBuffer;

    GLuint worldBufferID;
    GLuint textureWindowID;

    Octree octree;

    RaycastShader *rayShader;
};


#endif //STRANGEGAME_VOXELRENDER_H
