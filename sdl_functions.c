#include "include/vars.h"
#include "include/sdl_functions.h"

int setup_sdl(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
  }
  *window = SDL_CreateWindow("Presento", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (*window == NULL) {
    return -1;
  }
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (*renderer == NULL) {
    SDL_DestroyWindow(*window);
    return -1;
  }
  return 0;
}
int setup_ttf(TTF_Font **regular, TTF_Font **bold) {
  if (TTF_Init() == -1) {
    return -1;
  }
  *regular = TTF_OpenFont("fonts/calibri-regular.ttf", REGULAR_PTSIZE);
  if (*regular == NULL) {
    return -1;
  }
  *bold = TTF_OpenFont("fonts/calibri-bold.ttf", BOLD_PTSIZE);
  if (*bold == NULL) {
    TTF_CloseFont(*regular);
    return -1;
  }
  return 0;
}

void close_sdl(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
void close_ttf(TTF_Font *regular, TTF_Font *bold) {
  TTF_CloseFont(bold);
  TTF_CloseFont(regular);
  TTF_Quit();
}

void push_quit() {
  SDL_Event ev = {.type=SDL_QUIT};
  SDL_PushEvent(&ev);
}