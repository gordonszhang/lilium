#include "vector.h"

Velocity::Velocity() : x(), y() {};
Velocity::Velocity(float velX, float velY) : x(velX), y(velY) {};
Velocity& Velocity::operator=(const Velocity& rhs) {
  if(this == &rhs) return *this;
  x = rhs.x;
  y = rhs.y;
  return *this;
}
