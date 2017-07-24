#include "slash.h"
#include "gamedata.h"
#include "renderContext.h"

void Slash::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Slash::Slash(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"Up/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"Up/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"Up/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"Up/speedY"))
           ),
  framesUp( RenderContext::getInstance()->getFrames(name+"Up") ),
	framesDown(RenderContext::getInstance()->getFrames(name+"Down")),
	framesLeft(RenderContext::getInstance()->getFrames(name+"Left")),
	framesRight(RenderContext::getInstance()->getFrames(name+"Right")),
	selectedFrames(framesUp),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"Up/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"Up/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(framesUp[0]->getWidth()),
  frameHeight(framesUp[0]->getHeight()),
	state(-1),
    direction(UP),
	isInvulnerable(),
	offFrame()

{ }

Slash::Slash(const Slash& s) :
  Drawable(s),
  framesUp(s.framesUp),
	framesDown(s.framesDown),
	framesLeft(s.framesLeft),
	framesRight(s.framesRight),
	selectedFrames(s.selectedFrames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
	state(s.state),
    direction(s.direction),
	isInvulnerable(s.isInvulnerable),
	offFrame(s.offFrame)
  { }

void Slash::draw() const {
    selectedFrames[currentFrame]->draw(getX(), getY());
}

void Slash::update(Uint32 ticks) {
  advanceFrame(ticks);

  //if(!isAlive()) offFrame = (offFrame + 1) % 4;
	//std::cout << "state: " << state << std::endl;
	
    switch(direction) {
        case UP:
            selectedFrames = framesUp;
            setName("slashUp");
            break;
        case DOWN:
            selectedFrames = framesDown;
            setName("slashDown");
            break;
        case LEFT:
            selectedFrames = framesLeft;
            setName("slashLeft");
            break;
        case RIGHT:
            selectedFrames = framesRight;
            setName("slashRight");
            break;
        default:
            break;
    }
}

void Slash::setAlive(bool a) {
  Drawable::setAlive(a);
  //if(!a) offFrame = 0;
}

void Slash::setDirection(int d) {
    direction = d;
}

int Slash::getDirection() {
    return direction;
}