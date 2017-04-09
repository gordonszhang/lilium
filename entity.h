#ifndef ENTITY_H
#define ENTITY_H

#include "vector.h"

class Entity {
  public:
    virtual ~Entity() {};
    virtual void update() = 0;
    const Position& getPosition()         { return p; };
    void setPosition(const Position& pos) { p = pos;  };
    const Velocity& getVelocity()         { return v; };
    void setVelocity(const Velocity& vel) { v = vel;  };

  protected:
    Entity() : p(), v() {}
    Position p;
    Velocity v;
};

#endif
