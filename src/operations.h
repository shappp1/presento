#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "vars.h"

void prev_slide(Program *prog);
void next_slide(Program *prog);
void move_slide_back(Program *prog);
void move_slide_forward(Program *prog);
void new_slide(Program *prog);
void delete_slide(Program *prog);
void switch_active_text_box(Slide *slide);
void new_text_box(Slide *slide, bool bold);
void delete_text_box(Slide *slide);
void move_text_box_up(TextBox *box);
void move_text_box_down(TextBox *box);
void move_text_box_left(TextBox *box);
void move_text_box_right(TextBox *box);
void vert_shrink_text_box(TextBox *box);
void vert_grow_text_box(TextBox *box);
void hor_shrink_text_box(TextBox *box);
void hor_grow_text_box(TextBox *box);

void handle_enter(char *text);
void handle_backspace(char *text);

int save(Program *prog, const char *file_name);
int load(Program *prog, const char *file_name);

void setup_slide(Slide *slide, int index, Slide *next_slide);
void setup_program(Program *prog);
void free_program(Program *prog);

#endif