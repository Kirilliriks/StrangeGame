//
// Created by KirilliriK on 28.02.2022.
//

#include "Voxel.h"

Voxel::Voxel(int r, int g, int b) {
    color = Color(r, g, b);
}

Color Voxel::getColor() {
    return color;
}
