#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "stuff.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main(int argc, char** argv) {
  // Will add error handlers for these later
  (void)SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Presento", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  Program program = {false};
  while (!program.should_quit) {
    SDL_SetRenderDrawColor(renderer, 255, 240, 215, SDL_ALPHA_OPAQUE); // will probably change this colour later
    SDL_RenderClear(renderer);

    handle_events(&program);

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 fps (doesn't really matter)
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void handle_events(Program *prog) {
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    switch (ev.type) {
      case SDL_QUIT:
        prog->should_quit = true;
        break;
      case SDL_KEYDOWN:
        switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE:
            push_quit();
            break;
        }
    }
  }
}
