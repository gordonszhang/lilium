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
  slashUp(RenderContext::getInstance()->getFrame("slashUp")),
	selectedFrames(frames),
	barrier(),
  enemy(),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"I/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"I/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
	state(0), actionState(0), nextState(0),
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
  slashUp(s.slashUp),
	selectedFrames(s.selectedFrames),
	barrier(s.barrier),
  enemy(s.enemy),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
	state(s.state), actionState(s.actionState), nextState(s.nextState),
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
  if(actionState == SLASH_A1 || actionState == SLASH_A2 
     || actionState == SLASH_B1 || actionState == SLASH_B2
     || actionState == SLASH_C1 || actionState == SLASH_C2) {
    slashUp->draw(getX(), getY());
  }
}

void Player::handleInput(SDL_Event event, const Uint8* keystate, Uint32 mousestate) {
    // "Slash" input
    if(event.type == SDL_MOUSEBUTTONDOWN) {
      if (mousestate && SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if(actionState == IDLE) {
          nextState = SLASH_A1;
          stateTimer = 0;
        }
        if(actionState == SLASH_A2) {
          nextState = SLASH_B1;
        }
        if(actionState == SLASH_B2) {
          nextState = SLASH_C1;
        }
      }
    }
    // Player control
    setVelocityX(0);
    setVelocityY(0);
    if ( keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D] ) {

    }
    else if ( keystate[SDL_SCANCODE_A] ) {
      setVelocityX(getVelocityX() - 200);
    }
    else if ( keystate[SDL_SCANCODE_D] ) {
      setVelocityX(getVelocityX() + 200);
    }
    if ( keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S] ) {

    }
    else if ( keystate[SDL_SCANCODE_W] ) {
      setVelocityY(getVelocityY() - 200);
    }
    else if ( keystate[SDL_SCANCODE_S] ) {
      setVelocityY(getVelocityY() + 200);
    }

  
}

char Player::getState() {
  return actionState;
}

void Player::update(Uint32 ticks) {
  advanceFrame(ticks);

  if(stateTimer == 0) {
    actionState = nextState;
    if(actionState == SLASH_A1) {
      nextState = SLASH_A2;
      stateTimer = 6;
    }
    else if(actionState == SLASH_A2) {
      nextState = SLASH_A3;
      stateTimer = 12;
    }
    else if(actionState == SLASH_A3) {
      nextState = IDLE;
      stateTimer = 4;
    }

    else if(actionState == SLASH_B1) {
      nextState = SLASH_B2;
      stateTimer = 6;
    }
    else if(actionState == SLASH_B2) {
      nextState = SLASH_B3;
      stateTimer = 12;
    }
    else if(actionState == SLASH_B3) {
      nextState = IDLE;
      stateTimer = 4;
    }

    else if(actionState == SLASH_C1) {
      nextState = SLASH_A2;
      stateTimer = 6;
    }
    else if(actionState == SLASH_C2) {
      nextState = SLASH_A3;
      stateTimer = 18;
    }
    else if(actionState == SLASH_C3) {
      nextState = IDLE;
      stateTimer = 4;
    }
    
  }

  --stateTimer;

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
