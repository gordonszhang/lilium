#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "barrier.h"
#include "sprite.h"

#define IDLE 0
#define SLASH_A1 1
#define MOVE 2

#define UP 100
#define DOWN 101
#define LEFT 102
#define RIGHT 103

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

  void attachEnemy(Drawable* b);
  void detachEnemy();

  void handleInput(SDL_Event event, const Uint8*, const Uint32);
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
  int stateTimer;
  bool movingRight;

  bool isInvulnerable;
  int offFrame;
  void advanceFrame(Uint32 ticks);
};
#endif
