//
// Created by KirilliriK on 28.02.2022.
//

#ifndef STRANGEGAME_VOXEL_H
#define STRANGEGAME_VOXEL_H


#include "../utils/Color.h"

class Voxel {
public:
    Voxel(int r, int g, int b);
    Color getColor();
private:
    Color color;
};


#endif //STRANGEGAME_VOXEL_H
