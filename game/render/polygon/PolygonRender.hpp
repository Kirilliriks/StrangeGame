//
// Created by KirilliriK on 17.02.2024.
//

#ifndef POLYGONRENDER_H
#define POLYGONRENDER_H
#include <vector>

#include "../shaders/Shader.hpp"
#include "object/Object.hpp"

class Mesh;
class Game;
class PolygonRender {
public:
    explicit PolygonRender(Game *game);
    ~PolygonRender() = default;

    void updateWorld();
    void render(double deltaTime) const;
private:
    Game &game;
    Shader shader;

    std::vector<Mesh> objects;
};



#endif //POLYGONRENDER_H
