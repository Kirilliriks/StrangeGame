//
// Created by KirilliriK on 28.02.2022.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "render/VoxelRender.h"

class Game {
public:
    Game();
    void update(double deltaTime);
    void render(double deltaTime);
private:
    VoxelRender renderer;
};


#endif //GAME_GAME_H
