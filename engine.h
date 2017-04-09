#include "world.h"

class Engine {
  public:
    Engine();
    ~Engine();
    void update();
  private:
    Engine(Engine&);
    Engine& operator=(Engine&);
    World* world;
};
