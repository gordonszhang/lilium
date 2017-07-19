#include "player.h"
#include "gamedata.h"
#include "renderContext.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"I/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"I/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"I/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"I/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name+"I") ),
	framesRight(RenderContext::getInstance()->getFrames(name+"R")),
	framesLeft(RenderContext::getInstance()->getFrames(name+"L")),
	framesTransitionRight(RenderContext::getInstance()->getFrames(name+"TR")),
	framesTransitionLeft(RenderContext::getInstance()->getFrames(name+"TL")),
  framesTurnRight(RenderContext::getInstance()->getFrames(name+"UR")),
	framesTurnLeft(RenderContext::getInstance()->getFrames(name+"UL")),
	selectedFrames(frames),
	barrier(),
  enemy(),
  slashUp(RenderContext::getInstance()->getFrame("slashUp")),
  slashDown(RenderContext::getInstance()->getFrame("slashDown")),
  slashRight(RenderContext::getInstance()->getFrame("slashRight")),
  slashLeft(RenderContext::getInstance()->getFrame("slashLeft")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"I/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"I/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
	state(0), actionState(0), nextState(0), direction(UP), moveVector(0, 0),
  stateTimer(0), movingRight(false),
  isInvulnerable(false), offFrame()
{ }

Player::Player(const Player& s) :
  Drawable(s),
  frames(s.frames),
	framesRight(s.framesRight),
	framesLeft(s.framesLeft),
	framesTransitionRight(s.framesTransitionRight),
	framesTransitionLeft(s.framesLeft),
  framesTurnRight(s.framesTurnRight),
	framesTurnLeft(s.framesTurnLeft),
	selectedFrames(s.selectedFrames),
	barrier(s.barrier),
  enemy(s.enemy),
  slashUp(s.slashUp), slashDown(s.slashDown),
  slashRight(s.slashRight), slashLeft(s.slashLeft),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
	state(s.state), actionState(s.actionState), nextState(s.nextState), direction(s.direction), moveVector(s.moveVector),
  stateTimer(s.stateTimer), movingRight(s.movingRight),
  isInvulnerable(s.isInvulnerable), offFrame(s.offFrame)
  { }

void Player::draw() const {
	barrier[0]->draw();
  if(isAlive()) {
    selectedFrames[currentFrame]->draw(getX(), getY());
  }
  else {
    if(offFrame > 1) {
      selectedFrames[currentFrame]->draw(getX(), getY());
    }
  }
  if(actionState == SLASH_A1) {
      if(direction == UP) {
        slashUp->draw(getX(), getY());
      }
      else if(direction == DOWN) {
        slashDown->draw(getX(), getY());
      }
      else if(direction == RIGHT) {
        slashRight->draw(getX(), getY());
      }
      else if(direction == LEFT) {
        slashLeft->draw(getX(), getY());
      }

  }
}

void Player::handleInput() {
  SDL_Event event;
  
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  // Player control
  if(actionState != SLASH_A1) {
    moveVector[0] = 0;
    moveVector[1] = 0;
		nextState = IDLE;
    if ( keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D] ) {

    }
    else if ( keystate[SDL_SCANCODE_A] ) {
      moveVector[0] = -1;
      if(actionState != MOVE || direction == RIGHT) direction = LEFT;
    }
    else if ( keystate[SDL_SCANCODE_D] ) {
      moveVector[0] = 1;
      if(actionState != MOVE || direction == LEFT) direction = RIGHT;
    }
    if ( keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S] ) {

    }
    else if ( keystate[SDL_SCANCODE_W] ) {
       moveVector[1] = -1;
      if(actionState != MOVE || direction == DOWN) direction = UP;
    }
    else if ( keystate[SDL_SCANCODE_S] ) {
       moveVector[1] = 1;
      if(actionState != MOVE || direction == UP) direction = DOWN;
    }

    if(moveVector[0] != 0 || moveVector[1] != 0) {
      nextState = MOVE;
    }
  }
  while(SDL_PollEvent(&event)) {
    // "Slash" input
    if(event.type == SDL_QUIT) { exit(0); }
    if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
      if (keystate[SDL_SCANCODE_J]) {
        actionState = SLASH_A1;
        nextState = IDLE;
        stateTimer = 12;
        //stateTimer = 0;
      }
      /*
      if (mousestate && SDL_BUTTON(SDL_BUTTON_LEFT)) {
        nextState = SLASH_A1;
        stateTimer = 0;
      }
      */
    }
  }
}

char Player::getState() {
  return actionState;
}

void Player::update(Uint32 ticks) {
  advanceFrame(ticks);

  if(stateTimer <= 0) {

		if(nextState == SLASH_A1) {
			std::cout << "nextState is " << (int)nextState << std::endl;
		}
    actionState = nextState;
    if(actionState == SLASH_A1) {
			std::cout << "hi im slashing" << std::endl;
      stateTimer = 12;
    }

    else if(actionState == MOVE) {
    }
  }

  if(stateTimer > -1) --stateTimer;

  setVelocityX(0);
  setVelocityY(0);

  if(actionState == MOVE) {
    setVelocityX(moveVector[0] * 200);
    setVelocityY(moveVector[1] * 200);
  }

  if(!isAlive()) offFrame = (offFrame + 1) % 4;
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setY(0);
  }
  if ( getY() > worldHeight-frameHeight) {
    setY(worldHeight-frameHeight);
  }

  if ( getX() < 0) {
    setX(0);
  }
  if ( getX() > worldWidth-frameWidth) {
    setX(worldWidth-frameWidth);
  }
	if(getVelocityX() > 0) {
		if(state < 0) {
			state = 1;
			selectedFrames = framesTransitionRight;
		}
    else if(state == 0) {
      state = 5;
      selectedFrames = framesTurnRight;
    }
		else if(state > 0 && state < 9) {
			++state;
		}
		else if (state == 9) {
			selectedFrames = framesRight;

			++state;
		}
	}
	if(getVelocityX() == 0) {
    state = 0;
    selectedFrames = frames;
  }
	if(getVelocityX() < 0) {
		if(state > 0) {
			state = -1;
			selectedFrames = framesTransitionLeft;
		}
    else if(state == 0) {
      state = -5;
      selectedFrames = framesTurnLeft;
    }
		else if(state < 0 && state > -9) {
			--state;
		}
		else if (state == -9) {
			selectedFrames = framesLeft;
			--state;
		}
	}
	barrier[0]->setX(getX());
	barrier[0]->setY(getY());
	barrier[0]->setVelocityX(getVelocityX());
	barrier[0]->setVelocityY(getVelocityY());
	barrier[0]->update(ticks);
  enemy[0]->updatePlayerPos(getPosition());
}

void Player::attachBarrier(Drawable *b) {
	barrier.push_back((Barrier*)b);
}

void Player::detachBarrier() {
	barrier.pop_back();
}

void Player::attachEnemy(Drawable *b) {
	enemy.push_back((Sprite*)b);
}

void Player::detachEnemy() {
	enemy.pop_back();
}


void Player::setAlive(bool a) {
  Drawable::setAlive(a);
	barrier[0]->setAlive(a);
  if(!a) offFrame = 0;
}
