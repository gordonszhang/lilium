#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "barrier.h"
#include "sprite.h"
#include "slash.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }

  virtual void setAlive(bool a);

	void attachBarrier(Drawable* b);
	void detachBarrier();

  void attachSlash(Drawable* s);
  void detachSlash();

  void attachEnemy(Drawable* b);
  void detachEnemy();

  void handleInput();
  char getState();

protected:
  const std::vector<Frame *> frames;
  const std::vector<Frame *> framesRight;
  const std::vector<Frame *> framesLeft;
  const std::vector<Frame *> framesTransitionRight;
  const std::vector<Frame *> framesTransitionLeft;
  const std::vector<Frame *> framesTurnRight;
  const std::vector<Frame *> framesTurnLeft;
  std::vector<Frame *> selectedFrames;
	std::vector<Barrier *> barrier;
  std::vector<Sprite *> enemy;
  std::vector<Slash *> slash;
  Frame* slashUp;
  Frame* slashDown;
  Frame* slashRight;
  Frame* slashLeft;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int state;
  char actionState;
  char nextState;
  char direction;
  Vector2f moveVector;
  int stateTimer;
  bool movingRight;

  bool isInvulnerable;
  int offFrame;
  void advanceFrame(Uint32 ticks);
};
#endif
