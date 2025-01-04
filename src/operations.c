#include "operations.h"

void prev_slide(Program *prog) {
  if (prog->active_slide->index != 0) {
    int prev_slide_index = prog->active_slide->index - 1;
    prog->active_slide = prog->first_slide;
    while (prog->active_slide->index != prev_slide_index)
      prog->active_slide = prog->active_slide->next_slide;
  }
}
void next_slide(Program *prog) {
  if (prog->active_slide->next_slide != NULL)
  prog->active_slide = prog->active_slide->next_slide;
}
void move_slide_back(Program *prog) {
  if (prog->active_slide->index != 0) {
    prev_slide(prog);
    move_slide_forward(prog);
    prev_slide(prog);
  }
}
void move_slide_forward(Program *prog) {
  if (prog->active_slide->next_slide != NULL) {
    if (prog->active_slide->index == 0) {
      prog->first_slide = prog->active_slide->next_slide;
      prog->active_slide->next_slide = prog->active_slide->next_slide->next_slide;
      prog->first_slide->next_slide = prog->active_slide;
      prog->first_slide->index--;
      prog->first_slide->next_slide->index++;
    } else {
      Slide *prev = prog->first_slide;
      while (prev->index != prog->active_slide->index - 1)
        prev = prev->next_slide;
      prev->next_slide = prog->active_slide->next_slide;
      prog->active_slide->next_slide = prog->active_slide->next_slide->next_slide;
      prev->next_slide->next_slide = prog->active_slide;
      prev->next_slide->index--;
      prog->active_slide->index++;
    }
  }
}
void new_slide(Program *prog) {
  Slide *next = prog->active_slide->next_slide;
  prog->active_slide->next_slide = (Slide *)malloc(sizeof *prog->active_slide->next_slide);
  setup_slide(prog->active_slide->next_slide, prog->active_slide->index + 1, next);
  prog->active_slide = prog->active_slide->next_slide;
  while (next != NULL) {
    next->index++;
    next = next->next_slide;
  }
  prog->slide_count++;
}
void delete_slide(Program *prog) {
  if (prog->active_slide->index == 0 && prog->active_slide->next_slide != NULL) {
    prog->first_slide = prog->active_slide->next_slide;
    free(prog->active_slide->text_boxes);
    free(prog->active_slide);
    Slide *dec_index = prog->first_slide;
    while (dec_index != NULL) {
      dec_index->index--;
      dec_index = dec_index->next_slide;
    }
    prog->active_slide = prog->first_slide;
    prog->slide_count--;
  } else if (prog->active_slide->index != 0) {
    Slide *prev = prog->first_slide;
    while (prev->index != prog->active_slide->index - 1)
      prev = prev->next_slide;
    prev->next_slide = prog->active_slide->next_slide;
    free(prog->active_slide->text_boxes);
    free(prog->active_slide);
    prog->active_slide = (prev->next_slide == NULL) ? prev : prev->next_slide;
    prev = prev->next_slide;
    while (prev != NULL) {
      prev->index--;
      prev = prev->next_slide;
    }
    prog->slide_count--;
  }
}
void switch_active_text_box(Slide *slide) {
  if (slide->text_box_count != 0) {
    slide->active_box_index = (slide->active_box_index + 1) % slide->text_box_count;
  }
}
void new_text_box(Slide *slide, bool bold) {
  if (slide->text_box_count < 8) {
    if (bold) {
      slide->text_boxes[slide->text_box_count++] = (TextBox){40, 40, 720, 60, "", true};
    } else {
      slide->text_boxes[slide->text_box_count++] = (TextBox){40, 120, 720, 290, "", false};
    }
    slide->active_box_index = slide->text_box_count - 1;
  }
}
void delete_text_box(Slide *slide) {
  if (slide->text_box_count != 0) {
    slide->text_box_count--;
    if (slide->active_box_index == slide->text_box_count) {
      slide->active_box_index = 0;
    } else for (int i = slide->active_box_index; i < slide->text_box_count; i++) {
      slide->text_boxes[i] = slide->text_boxes[i + 1];
    }
  }
}
void move_text_box_up(TextBox *box) {
  box->y -= (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->y < 0)
    box->y = 0;
}
void move_text_box_down(TextBox *box) {
  box->y += (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->y + box->h > EDIT_SLIDE_BOX_H)
    box->y = EDIT_SLIDE_BOX_H - box->h;
}
void move_text_box_left(TextBox *box) {
  box->x -= (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->x < 0)
    box->x = 0;
}
void move_text_box_right(TextBox *box) {
  box->x += (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->x + box->w > EDIT_SLIDE_BOX_W)
    box->x = EDIT_SLIDE_BOX_W - box->w;
}
void vert_shrink_text_box(TextBox *box) {
  box->h -= (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->h < 1)
    box->h = 1;
}
void vert_grow_text_box(TextBox *box) {
  box->h += (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->y + box->h > EDIT_SLIDE_BOX_H)
    box->h = EDIT_SLIDE_BOX_H - box->y;
}
void hor_shrink_text_box(TextBox *box) {
  box->w -= (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->w < 1)
    box->w = 1;
}
void hor_grow_text_box(TextBox *box) {
  box->w += (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
  if (box->x + box->w > EDIT_SLIDE_BOX_W)
    box->w = EDIT_SLIDE_BOX_W - box->x;
}

void handle_enter(char *text) {
  int index = strlen(text);
  if (index < TEXT_BUFFER_SIZE - 1) {
    text[index] = '\n';
    text[index + 1] = '\0';
  }
}

void handle_backspace(char *text) {
  if (strlen(text) != 0) {
    int index = strlen(text) - 1;
    if (text[index] & 0x80) {
      index++;
      while ((text[--index] & 0xC0) != 0xC0);
    }
    text[index] = '\0';
  }
}

void setup_slide(Slide *slide, int index, Slide *next_slide) {
  slide->text_boxes = (TextBox *)malloc(8 * sizeof *slide->text_boxes);
  slide->index = index;
  slide->next_slide = next_slide;
  slide->text_boxes[0] = (TextBox){40, 40, 720, 60, "", true};
  slide->text_boxes[1] = (TextBox){40, 120, 720, 290, "", false};
  slide->text_box_count = 2;
  slide->active_box_index = 0;
}
void setup_program(Program *prog) {
  *prog = (Program){NULL, (Slide *)malloc(sizeof *prog->first_slide), 1, false};
  setup_slide(prog->first_slide, 0, NULL);
  prog->active_slide = prog->first_slide;
}
void free_program(Program *prog) {
  Slide *current_slide = prog->first_slide;
  Slide *to_free;
  while (current_slide->next_slide != NULL) {
    free(current_slide->text_boxes);
    to_free = current_slide;
    current_slide = current_slide -> next_slide;
    free(to_free);
  }
}