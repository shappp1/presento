#ifndef VARS_H
#define VARS_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define EDIT_SLIDE_BOX_X 440
#define EDIT_SLIDE_BOX_Y 230
#define EDIT_SLIDE_BOX_W 800
#define EDIT_SLIDE_BOX_H 450

#define SLIDESHOW_SCALE 1.6

#define TEXT_BUFFER_SIZE 512
#define PATH_BUFFER_SIZE 128

#define REGULAR_PTSIZE 30
#define BOLD_PTSIZE 40

#define REGULAR_FONT_PATH "fonts/calibri-regular.ttf"
#define BOLD_FONT_PATH "fonts/calibri-bold.ttf"

typedef struct TextBox { 
  int x, y, w, h;
  char text[TEXT_BUFFER_SIZE];
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
  Slide *active_slide;
  Slide *first_slide;
  int slide_count;
  bool should_quit;
  bool slideshow_mode;
  bool must_save;
  bool must_load;
  int x_off, y_off;
} Program;

#endif