//
// Created by KirilliriK on 17.02.2024.
//

#ifndef POLYGONRENDER_H
#define POLYGONRENDER_H
#include <vector>

#include "../../world/TraceStack.hpp"
#include "../shaders/Shader.hpp"
#include "object/Object.hpp"

class Mesh;
class Game;
class PolygonRender {
public:
    explicit PolygonRender(Game *game);
    ~PolygonRender() = default;

    void traceLine(const TraceStack& traceStack);
    void updateWorld();
    void update();
    void render(const double& deltaTime) const;
    void clearObjects();
private:
    Game &game;
    Shader shader;

    Object world;
    std::vector<Object> objects;
};



#endif //POLYGONRENDER_H
