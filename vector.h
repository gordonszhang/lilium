#ifndef VECTOR_H
#define VECTOR_H

struct Position {
  int x;
  int y;
};

struct Velocity {
  Velocity();
  Velocity(float, float);
  Velocity& operator=(const Velocity&);
  float x;
  float y;
};

#endif
