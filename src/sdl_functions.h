#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int setup_sdl(SDL_Window **window, SDL_Renderer **renderer);
int setup_ttf(TTF_Font **regular, TTF_Font **bold, TTF_Font **regular_ss, TTF_Font **bold_ss);

int write_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);
int write_text_wrapped(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, unsigned int wrap_length, int x, int y);

void close_sdl(SDL_Window *window, SDL_Renderer *renderer);
void close_ttf(TTF_Font *regular, TTF_Font *bold, TTF_Font *regular_ss, TTF_Font *bold_ss);

void push_quit();

#endif