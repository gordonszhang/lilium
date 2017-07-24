#ifndef SLASH_H
#define SLASH_H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Slash : public Drawable {
public:
  Slash(const std::string&);
  Slash(const Slash&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return selectedFrames[currentFrame];
  }

  virtual void setAlive(bool a);
    void setDirection(int d);

protected:
  const std::vector<Frame *> framesUp;
  const std::vector<Frame *> framesDown;
  const std::vector<Frame *> framesLeft;
  const std::vector<Frame *> framesRight;
  std::vector<Frame *> selectedFrames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int state;
  int direction;
  
  bool isInvulnerable;
  int offFrame;
  void advanceFrame(Uint32 ticks);
};
#endif
