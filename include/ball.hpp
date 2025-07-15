#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "common.hpp"
#include "spaceship.hpp"


class Ball {
 private:
  Point position, d;
  float rayon, vitesse;
  bool mouvement, falling;
  ALLEGRO_COLOR color;

 public:
  Ball();
  Ball(float rayon, float vitesse, ALLEGRO_COLOR color);

  // Drawing and movement
  void draw() const;
  void move(const Point& spaceship, float spaceship_height);
  void move(const Point& spaceship, float w, float h);
  
  // Collision and state management
  void checkCollisions(const Point& spaceship, float w, float h);
  void checkFall();
  void reset();
  
  // Getters
  Point getPosition()  const;
  Point getDirection() const;
  bool inMouvement()   const;
  bool isFalling()     const;
  float getRadius()    const;
  float getSpeed()     const;
  
  // Setters
  void setMouvement(bool cas);
  void setFalling(  bool cas);
  void setDirection(const Point& newDirection);
  void setPosition( const Point& newPosition);
};

#endif
