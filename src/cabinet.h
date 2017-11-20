#ifndef CABINET_H
#define CABINET_H

#include <SDL2/SDL.h>

#include "space_invaders.h"

class Cabinet
{
  public:
    Cabinet();
    void bootstrap();

  private:
    SpaceInvaders hardware;
    CPU cpu;
    SDL_Renderer *renderer;
    void loadROM();
    void initDisplay();
    void initCPU();
    void mainLoop();
};

#endif
