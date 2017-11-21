#include <chrono>
#include <cstdint>
#include <stdio.h>
#include <string>
#include <sys/time.h>

#include "cabinet.h"
#include "io.h"
#include "op_codes.h"

#define FILE_SIZE 8192
#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256

using namespace std;
using namespace std::chrono;

Cabinet::Cabinet() : renderer(NULL)
{
}

void Cabinet::bootstrap()
{
  loadROM();
  initCPU();
  initDisplay();
}

void Cabinet::loadROM()
{
  uint8_t buffer[FILE_SIZE];
  string inputFile = "data/invaders.bin";
  memset(buffer, 0, FILE_SIZE);

  if (openFile(inputFile, buffer, FILE_SIZE) == 0) 
  {
    cpu.loadProgram(buffer, FILE_SIZE);
  }
  else
  {
    printf("Failed to load %s\n", inputFile.c_str());
    exit(0);
  }
}

void Cabinet::initCPU()
{
  cpu.stepThrough = true;
  cpu.setPortHandler(&hardware);
}

void Cabinet::initDisplay()
{
  SDL_Window *window = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("Cannot init SDL: %s", SDL_GetError());
  }
  else
  {
    window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
      printf("SDL Window could not be created: %s", SDL_GetError());
    }
    else
    {
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

      if (!renderer)
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
      }
      else
      {
        mainLoop();
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Cabinet::mainLoop()
{
  bool running = true;
  bool vsync1 = true;
  SDL_Event event;
  long long now = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
  long long last = now;
  long long nextInterrupt = now + 8333;
  int cyclesPerMicrosecond = 2;

  while (running) 
  {
    now = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();

    if (now > nextInterrupt)
    {
      cpu.handleInterrupt(vsync1 ? RST_1 : RST_2);
      vsync1 = !vsync1;
      nextInterrupt = now + 8333;
    }

    uint32_t elapsedTime = now - last;
    uint32_t targetCycles = elapsedTime * cyclesPerMicrosecond;
    cpu.resetElapsedCycles();

    while (cpu.elapsedCycles() < targetCycles)
    {
      cpu.processProgram();
    }

    while (SDL_PollEvent(&event)) 
    {
      if (event.type == SDL_QUIT) 
      {
        running = false;
      }
    }

    SDL_RenderClear(renderer);

    //57344 = bits in video
    //7168 = bytes in video
    //224 x 256 = resolution bits
    //28 x 32 = resolution bytes

    int index = 0;
    for (vector<uint8_t>::iterator it = cpu.memory.begin() + (0x4000 - 1); it >= cpu.memory.begin() + 0x2400; --it) {

      for (int p=7; p>=0; p--)
      {
        if (*it & (1<<p))
        {
          SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        }
        else
        {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }

        int originalX = index % 256;
        int originalY = index / 256;
        int updatedX = originalY;
        int updatedY = 256 - originalX;
        SDL_Rect fillRect = { 224 - updatedX, 256 - updatedY, 1, 1 };
        SDL_RenderFillRect(renderer, &fillRect);
        index++;
      }
    }

    SDL_RenderPresent(renderer);
    last = now;
  }
}
