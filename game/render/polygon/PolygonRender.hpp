//
// Created by KirilliriK on 17.02.2024.
//

#ifndef POLYGONRENDER_H
#define POLYGONRENDER_H
#include <vector>

#include "../../world/octree/TraceStack.hpp"
#include "../shaders/Shader.hpp"
#include "object/Object.hpp"

class Mesh;
class Game;
class PolygonRender {
public:
    explicit PolygonRender(Game *game);
    ~PolygonRender() = default;

    void traceLine(const TraceStack& testStack, const TraceStack& traceStack);
    void rebuildWorld();
    void update();
    void imgui();
    void render(const double& deltaTime) const;
    void clearObjects();
private:
    Game &game;
    Shader shader;

    TraceStack testTrace;
    TraceStack lastTrace;

    Object world;
    std::vector<Object> objects;
};



#endif //POLYGONRENDER_H
