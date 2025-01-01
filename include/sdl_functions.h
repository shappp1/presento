#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int setup_sdl(SDL_Window **window, SDL_Renderer **renderer);
int setup_ttf(TTF_Font **regular, TTF_Font **bold);
void close_sdl(SDL_Window *window, SDL_Renderer *renderer);
void close_ttf(TTF_Font *regular, TTF_Font *bold);
void push_quit();

#endif