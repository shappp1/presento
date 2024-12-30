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
  int index;
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
static inline void setup_slide(Slide *slide, int index, Slide *next_slide) {
  slide->text_boxes = (TextBox *)malloc(8 * sizeof *slide->text_boxes);
  slide->index = index;
  slide->next_slide = next_slide;
  slide->text_boxes[0] = (TextBox){{40, 40, 720, 80}, {0}, true};
  slide->text_boxes[1] = (TextBox){{40, 160, 720, 250}, {0}, false};
  slide->text_box_count = 2;
  slide->active_box_index = 0;
}
void free_program(Program *prog);

#endif