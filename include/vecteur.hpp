#ifndef VECTEUR_HPP
#define VECTEUR_HPP

#include "common.hpp"
#include "rectangle.hpp"

class Vecteur {
private:
    Point position;
    int w, h;
    // Coordonnées
    Point top_left = {position.x - w/2, position.y - h/2};
    Point top_right = {position.x + w/2, position.y - h/2};
    Point bottom_left = {position.x - w/2, position.y + h/2};
    Point bottom_right = {position.x + w/2, position.y + h/2};
    // Vecteur
    pair<Point, Point> top_edge = make_pair(top_left, top_right);
    pair<Point, Point> bottom_edge = make_pair(bottom_left, bottom_right);
    pair<Point, Point> left_edge = make_pair(top_left, bottom_left);
    pair<Point, Point> right_edge = make_pair(top_right, bottom_right);

public:
    Vecteur(Point position, int w, int h);
};

#endif