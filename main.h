#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>

typedef struct TextBox {
  SDL_Rect dimensions;
  char text[256]; // char count may change
  bool bold;
} TextBox;

typedef struct Slide { // slides are held in linked list
  TextBox *text_boxes;
  int active_box_index;
  int text_box_count;
  int slide_no;
  struct Slide *next_slide;
} Slide;

typedef struct Program {
  Slide *current_slide;
  Slide *first_slide;
  int slide_count;
  bool should_quit;
} Program;

void handle_events(Program *prog);
static inline void push_quit() {
  SDL_Event ev = {.type=SDL_QUIT};
  SDL_PushEvent(&ev);
}

#endif