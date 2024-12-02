#ifndef BRICK_HPP
#define BRICK_HPP

#include "common.hpp"
#include "rectangle.hpp"
#include "vecteur.hpp"

class Brick : public Rectangle {
private:
    int scores;
    bool destroyed;
    Vecteur vec;

public:
    Brick(Point position, float w, float h, 
          ALLEGRO_COLOR frameColor, 
          ALLEGRO_COLOR fillColor, 
          int scores);
    int destroy();
    bool isDestroyed();
};

#endif
