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
#include "../world/World.h"

class Game;
class VoxelRender {
public:
    VoxelRender(Game *game);
    void createWorld();
    void updateWorld();
    void update(double deltaTime);
    void render(double deltaTime);
    void imgui(double deltaTime);

    GLuint genTexture();
private:
    Game *game;
    Camera &camera;
    Window *window;

    GLuint raycastShaderID;
    Shader shader;

    GLuint windowArrayID;
    GLuint windowBuffer;

    GLuint worldBufferID;

    World world;

    RaycastShader *rayShader;
    Octree::DebugCast debugCast;
    glm::ivec3 frontVoxel;

    int groupSize = 8;
};


#endif //STRANGEGAME_VOXELRENDER_H
