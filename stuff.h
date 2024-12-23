// truly brilliant file name ik
#ifndef STUFF_H
#define STUFF_H

#include <SDL2/SDL.h>

typedef struct TextBox {
  int x;
  int y;
  int width;
  int height;
  char text[256];
  bool bold;
} TextBox;

typedef struct Slide {
  TextBox text_boxes[5];
  struct Slide *next_slide;
} Slide;

typedef struct Program {
  bool should_quit;
  Slide *first_slide;
  Slide *current_slide;
} Program;

void handle_events(Program *prog);
static inline void push_quit() {
  SDL_Event ev = {.type=SDL_QUIT};
  SDL_PushEvent(&ev);
}

#endif