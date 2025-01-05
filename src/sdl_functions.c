#include "vars.h"
#include "sdl_functions.h"

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
int setup_ttf(TTF_Font **regular, TTF_Font **bold, TTF_Font **regular_ss, TTF_Font **bold_ss) {
  if (TTF_Init() == -1) {
    return -1;
  }
  *regular = TTF_OpenFont(REGULAR_FONT_PATH, REGULAR_PTSIZE);
  if (*regular == NULL) {
    return -1;
  }
  *bold = TTF_OpenFont(BOLD_FONT_PATH, BOLD_PTSIZE);
  if (*bold == NULL) {
    TTF_CloseFont(*regular);
    return -1;
  }
  *regular_ss = TTF_OpenFont(REGULAR_FONT_PATH, REGULAR_PTSIZE * SLIDESHOW_SCALE);
  if (*bold == NULL) {
    TTF_CloseFont(*bold);
    TTF_CloseFont(*regular);
    return -1;
  }
  *bold_ss = TTF_OpenFont(BOLD_FONT_PATH, BOLD_PTSIZE * SLIDESHOW_SCALE);
  if (*bold == NULL) {
    TTF_CloseFont(*regular_ss);
    TTF_CloseFont(*bold);
    TTF_CloseFont(*regular);
    return -1;
  }
  return 0;
}

int write_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y) {
  SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
  if (surface == NULL) {
    return -1;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL) {
    SDL_FreeSurface(surface);
    return -1;
  }
  if (SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){x, y, surface->w, surface->h}) < 0) {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return -1;
  }
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  return 0;
}
int write_text_wrapped(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, unsigned int wrap_length, int x, int y) {
  SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, wrap_length);
  if (surface == NULL) {
    return -1;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL) {
    SDL_FreeSurface(surface);
    return -1;
  }
  if (SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){x, y, surface->w, surface->h}) < 0) {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return -1;
  }
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  return 0;
}

void close_sdl(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
void close_ttf(TTF_Font *regular, TTF_Font *bold, TTF_Font *regular_ss, TTF_Font *bold_ss) {
  TTF_CloseFont(bold_ss);
  TTF_CloseFont(regular_ss);
  TTF_CloseFont(bold);
  TTF_CloseFont(regular);
  TTF_Quit();
}

void push_quit() {
  SDL_Event ev = {.type=SDL_QUIT};
  SDL_PushEvent(&ev);
}