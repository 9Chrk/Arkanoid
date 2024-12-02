#ifndef BALL_HPP
#define BALL_HPP

#include "common.hpp"
#include "spaceship.hpp"

class Ball {
private:
    int rayon, vitesse;
    ALLEGRO_COLOR color;
    Point position, d;
    bool inMouvement, isFalling;

public:
    Ball(Point position, int rayon, int vitesse, ALLEGRO_COLOR color);
    void draw();
    void move(Point spaceship);
    void move(Point spaceship, float w, float h);
    Point getPosition();
    Point getDirection();
    void setDirection(Point newDirection);
    int getRayon();
    void checkCollisions(Point spaceship, float w, float h);
    void checkFall();
    void reset();
};

#endif
