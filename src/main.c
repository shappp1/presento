#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "vars.h"
#include "sdl_functions.h"
#include "operations.h"

#define ACTIVE_TEXT_BOX(prog) (prog).active_slide->text_boxes[(prog).active_slide->active_box_index]

void handle_events(Program *prog, SDL_Window *window, char *path);

int main(int argc, char** argv) {
  // Will add error handlers for these later
  SDL_Window *window;
  SDL_Renderer *renderer;
  if (setup_sdl(&window, &renderer) == -1) {
    printf("%s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  TTF_Font *regular_font, *bold_font, *regular_font_ss, *bold_font_ss;
  if (setup_ttf(&regular_font, &bold_font, &regular_font_ss, &bold_font_ss) == -1) {
    printf("%s\n", SDL_GetError());
    close_sdl(window, renderer);
    return EXIT_FAILURE;
  }

  Program program;
  setup_program(&program);

  Uint64 start_time, delta_time;
  const SDL_Rect edit_slide_rect = {EDIT_SLIDE_BOX_X, EDIT_SLIDE_BOX_Y, EDIT_SLIDE_BOX_W, EDIT_SLIDE_BOX_H};
  const int mspf = 16; // ~60 fps

  SDL_Surface *text_surface;
  SDL_Texture *text_texture;
  const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
  char slide_count_text[14], text_box_count_text[13], position_text[16], size_text[17];
  const char *controls_text = "Ctrl+S - save\nCtrl+L - load\nPgUp/PgDn - change slide\nCtrl+PgUp/PgDn - move slide\nCtrl+N - new slide\nCtrl+Delete - delete slide\nCtrl+P - slideshow (current)\nShift+Ctrl+P - slideshow (start)\nArrows - move text box*\nCtrl+Arrows - resize text box*\nTab - change active text box\nCtrl+T - new regular text box\nCtrl+B - new bold text box\nDel - delete active text box\n\n*Hold Shift to do faster";
  char path[PATH_BUFFER_SIZE] = "";
  SDL_StartTextInput();
  while (!program.should_quit) {
    start_time = SDL_GetTicks64();
    
    handle_events(&program, window, path);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    if (program.slideshow_mode) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(renderer, &(SDL_Rect){program.x_off, program.y_off, WINDOW_WIDTH, WINDOW_HEIGHT});

      for (int i = 0; i < program.active_slide->text_box_count; i++) {
        if (strlen(program.active_slide->text_boxes[i].text)) { // if textbox not empty
          write_text_wrapped(renderer, program.active_slide->text_boxes[i].bold ? bold_font_ss : regular_font_ss, program.active_slide->text_boxes[i].text, BLACK, program.active_slide->text_boxes[i].w * SLIDESHOW_SCALE, program.active_slide->text_boxes[i].x * SLIDESHOW_SCALE + program.x_off, program.active_slide->text_boxes[i].y * SLIDESHOW_SCALE + program.y_off);
        }
      }
    } else {
      SDL_SetRenderDrawColor(renderer, 255, 236, 210, SDL_ALPHA_OPAQUE); // might change this colour later
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(renderer, &edit_slide_rect);

      (void)write_text(renderer, bold_font, "Controls:", BLACK, 40, 40);
      (void)write_text_wrapped(renderer, regular_font, controls_text, BLACK, 0, 40, 90);

      snprintf(slide_count_text, sizeof slide_count_text, "Slide %d/%d", program.active_slide->index + 1, program.slide_count);
      (void)write_text(renderer, regular_font, slide_count_text, BLACK, EDIT_SLIDE_BOX_X + 20, 110);

      if (program.active_slide->text_box_count != 0) {
        snprintf(text_box_count_text, sizeof text_box_count_text, "Text box %d/%d", program.active_slide->active_box_index + 1, program.active_slide->text_box_count);
        (void)write_text(renderer, regular_font, text_box_count_text, BLACK, EDIT_SLIDE_BOX_X + 20, 150);

        if (ACTIVE_TEXT_BOX(program).bold)
          (void)write_text(renderer, regular_font, "Bold", BLACK, EDIT_SLIDE_BOX_X + 20, 190);

        snprintf(position_text, sizeof position_text, "pos:   %d  %d", ACTIVE_TEXT_BOX(program).x, ACTIVE_TEXT_BOX(program).y);
        (void)write_text(renderer, regular_font, position_text, BLACK, EDIT_SLIDE_BOX_X + 500, 110);

        snprintf(size_text, sizeof size_text, "size:   %d  %d", ACTIVE_TEXT_BOX(program).w, ACTIVE_TEXT_BOX(program).h);
        (void)write_text(renderer, regular_font, size_text, BLACK, EDIT_SLIDE_BOX_X + 500, 150);
      }

      if (program.must_save || program.must_load) {
        char path_input_text[PATH_BUFFER_SIZE + 6] = "Path: ";
        strcat(path_input_text, path);
        (void)write_text(renderer, regular_font, path_input_text, BLACK, EDIT_SLIDE_BOX_X + 20, 50);
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

      if (program.active_slide->text_box_count != 0) {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){ACTIVE_TEXT_BOX(program).x + EDIT_SLIDE_BOX_X, ACTIVE_TEXT_BOX(program).y + EDIT_SLIDE_BOX_Y, ACTIVE_TEXT_BOX(program).w, ACTIVE_TEXT_BOX(program).h});
      }

      for (int i = 0; i < program.active_slide->text_box_count; i++) {
        if (strlen(program.active_slide->text_boxes[i].text)) { // if textbox not empty
          write_text_wrapped(renderer, program.active_slide->text_boxes[i].bold ? bold_font : regular_font, program.active_slide->text_boxes[i].text, BLACK, program.active_slide->text_boxes[i].w, program.active_slide->text_boxes[i].x + EDIT_SLIDE_BOX_X, program.active_slide->text_boxes[i].y + EDIT_SLIDE_BOX_Y);
        }
      }
    }

    SDL_RenderPresent(renderer);

    delta_time = SDL_GetTicks64() - start_time;
    if (mspf > delta_time)
      SDL_Delay(mspf - delta_time);
    delta_time = SDL_GetTicks64() - start_time;
  }
  SDL_StopTextInput();

  free_program(&program);
  close_ttf(regular_font, bold_font, regular_font_ss, bold_font_ss);
  close_sdl(window, renderer);
  return EXIT_SUCCESS;
}

void handle_events(Program *prog, SDL_Window *window, char *path) {
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
  if (ev.type == SDL_QUIT) {
    prog->should_quit = true;
  } else if (ev.type == SDL_WINDOWEVENT) {
    if (ev.window.event = SDL_WINDOWEVENT_RESIZED) {
      int w, h;
      SDL_GetWindowSize(window, &w, &h);
      prog->x_off = (w > WINDOW_WIDTH) ? (w - WINDOW_WIDTH) >> 1 : 0;
      prog->y_off = (h > WINDOW_HEIGHT) ? (h - WINDOW_HEIGHT) >> 1 : 0;
    }
  } else if (ev.type == SDL_KEYDOWN) {
  if (prog->must_save || prog->must_load) {
  switch (ev.key.keysym.sym) {
    case SDLK_RETURN:
      if (prog->must_save)
        save(prog, path);
      else if (prog->must_load)
        load(prog, path);
    case SDLK_ESCAPE:
      path[0] = '\0';
      prog->must_save = false;
      prog->must_load = false;
      break;
    case SDLK_BACKSPACE:
      handle_backspace(path);
      break;
  }
  } else if (prog->slideshow_mode) {
  switch (ev.key.keysym.sym) { // for when I implement this
    case SDLK_LEFT:
      prev_slide(prog);
      break;
    case SDLK_RIGHT:case SDLK_SPACE:
      next_slide(prog);
      break;
    case SDLK_ESCAPE:
      prog->slideshow_mode = false;
      SDL_SetWindowFullscreen(window, 0);
      break;
  }
  } else {
  if (ev.key.keysym.mod & KMOD_CTRL) {
  switch (ev.key.keysym.sym) { // CTRL + sym
    case SDLK_PAGEUP:
      move_slide_back(prog);
      break;
    case SDLK_PAGEDOWN:
      move_slide_forward(prog);
      break;
    case SDLK_n:
      new_slide(prog);
      break;
    case SDLK_DELETE:
      delete_slide(prog);
      break;
    case SDLK_p:
      if (ev.key.keysym.mod & KMOD_SHIFT)
        prog->active_slide = prog->first_slide;
      prog->slideshow_mode = true;
      SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
      break;
    case SDLK_t:
      new_text_box(prog->active_slide, false);
      break;
    case SDLK_b:
      new_text_box(prog->active_slide, true);
      break;
    case SDLK_UP:
      if (prog->active_slide->text_box_count != 0)
        vert_shrink_text_box(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_DOWN:
      if (prog->active_slide->text_box_count != 0)
        vert_grow_text_box(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_LEFT:
      if (prog->active_slide->text_box_count != 0)
        hor_shrink_text_box(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_RIGHT:
      if (prog->active_slide->text_box_count != 0)
        hor_grow_text_box(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_s:
      prog->must_save = true;
      break;
    case SDLK_l:
      prog->must_load = true;
      break;
  }
  } else {
  switch (ev.key.keysym.sym) {
    case SDLK_PAGEUP:
      prev_slide(prog);
      break;
    case SDLK_PAGEDOWN:
      next_slide(prog);
      break;
    case SDLK_TAB:
      switch_active_text_box(prog->active_slide);
      break;
    case SDLK_DELETE:
      delete_text_box(prog->active_slide);
      break;
    case SDLK_UP:
      if (prog->active_slide->text_box_count != 0)
        move_text_box_up(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_DOWN:
      if (prog->active_slide->text_box_count != 0)
        move_text_box_down(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_LEFT:
      if (prog->active_slide->text_box_count != 0)
        move_text_box_left(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_RIGHT:
      if (prog->active_slide->text_box_count != 0)
        move_text_box_right(&ACTIVE_TEXT_BOX(*prog));
      break;
    case SDLK_RETURN:
      handle_enter(ACTIVE_TEXT_BOX(*prog).text);
      break;
    case SDLK_BACKSPACE:
      handle_backspace(ACTIVE_TEXT_BOX(*prog).text);
      break;
  }
  }
  }
  } else if (ev.type == SDL_TEXTINPUT && !prog->slideshow_mode) {
    if (prog->must_save || prog->must_load) {
      strncat(path, ev.text.text, PATH_BUFFER_SIZE - strlen(path) - 1);
    } else {
      strncat(ACTIVE_TEXT_BOX(*prog).text, ev.text.text, TEXT_BUFFER_SIZE - strlen(ACTIVE_TEXT_BOX(*prog).text) - 1);
    }
  }
  }
}

