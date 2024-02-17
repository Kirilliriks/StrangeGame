//
// Created by KirilliriK on 17.02.2024.
//

#ifndef POLYGONRENDER_H
#define POLYGONRENDER_H


class Game;
class PolygonRender {
public:
    explicit PolygonRender(Game *game);
    ~PolygonRender() = default;

    void updateWorld();
    void render(double deltaTime);
};



#endif //POLYGONRENDER_H
