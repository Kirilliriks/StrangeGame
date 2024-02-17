//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_VOXELRENDER_H
#define STRANGEGAME_VOXELRENDER_H
#define GLFW_INCLUDE_NONE

#include "../shaders/Shader.hpp"
#include "../shaders/RaycastShader.hpp"
#include "../../world/World.hpp"

class Game;
class World;
class VoxelRender {
public:
    VoxelRender(Game *game);
    ~VoxelRender() = default;

    void updateWorld();
    void render(double deltaTime);

    GLuint genTexture();
private:
    Game *game;
    World *world;
    Window *window;

    GLuint raycastShaderID;
    Shader shader;

    GLuint windowArrayID;
    GLuint windowBufferID;
    GLuint worldBufferID;

    RaycastShader *rayShader;

    int groupSize = 8;
};


#endif //STRANGEGAME_VOXELRENDER_H
