#ifndef HUD_H
#define HUD_H

#include <sstream>
#include "ioMod.h"
#include "clock.h"
#include "renderContext.h"
#include "SDL2/SDL.h"

class HUD {
public:
  void draw() const;
  HUD();
  void updateCounts(int alive, int free);
  void updateCollisions(int c);
  void updateBoss(int health);
  void updateDeaths(int deaths);
  void updateReticle(float mouse_x, float mouse_y, float player_x, float player_y);
  void updateState(char state);
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  int aliveBullets;
  int freeBullets;
  int collisions;
  int bossHealth;
  int deaths;
  float mouse_x;
  float mouse_y;
  float player_x;
  float player_y;
  int state;
};

#endif
