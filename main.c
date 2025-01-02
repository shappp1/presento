#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "include/vars.h"
#include "include/sdl_functions.h"
#include "include/operations.h"

#define ACTIVE_TEXT_BOX(prog) (prog).active_slide->text_boxes[(prog).active_slide->active_box_index]

void handle_events(Program *prog);

int main(int argc, char** argv) {
  // Will add error handlers for these later
  SDL_Window *window;
  SDL_Renderer *renderer;
  if (setup_sdl(&window, &renderer) == -1) {
    printf("%s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  TTF_Font *regular_font;
  TTF_Font *bold_font;
  if (setup_ttf(&regular_font, &bold_font) == -1) {
    printf("%s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  Program program;
  setup_program(&program);

  Uint64 start_time, delta_time;
  const SDL_Rect edit_slide_rect = {EDIT_SLIDE_BOX_X, EDIT_SLIDE_BOX_Y, EDIT_SLIDE_BOX_W, EDIT_SLIDE_BOX_H};
  const int mspf = 16; // 62.5 fps

  SDL_Surface *surface;
  SDL_Texture *texture;
  const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
  SDL_StartTextInput();
  while (!program.should_quit) {
    start_time = SDL_GetTicks64();
    
    handle_events(&program);

    SDL_SetRenderDrawColor(renderer, 255, 236, 210, SDL_ALPHA_OPAQUE); // might change this colour later
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &edit_slide_rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    if (program.active_slide->text_box_count != 0) {
      SDL_Rect text_box_rect = ACTIVE_TEXT_BOX(program).dimensions;
      text_box_rect.x += EDIT_SLIDE_BOX_X;
      text_box_rect.y += EDIT_SLIDE_BOX_Y;
      SDL_RenderDrawRect(renderer, &text_box_rect);
    }

    for (int i = 0; i < program.active_slide->text_box_count; i++) {
      if (strlen(program.active_slide->text_boxes[i].text)) { // if textbox not empty
        // needs error handling
        surface = TTF_RenderText_Blended_Wrapped(program.active_slide->text_boxes[i].bold ? bold_font : regular_font, program.active_slide->text_boxes[i].text, BLACK, program.active_slide->text_boxes[i].dimensions.w);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){program.active_slide->text_boxes[i].dimensions.x + EDIT_SLIDE_BOX_X, program.active_slide->text_boxes[i].dimensions.y + EDIT_SLIDE_BOX_Y, surface->w, surface->h});
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
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
  close_ttf(regular_font, bold_font);
  close_sdl(window, renderer);
  return EXIT_SUCCESS;
}

void handle_events(Program *prog) {
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
  if (ev.type == SDL_QUIT) {
    prog->should_quit = true;
  } else if (ev.type == SDL_KEYDOWN) {
  if (prog->slideshow_mode) {
  switch (ev.key.keysym.sym) { // for when I implement this
    case SDLK_LEFT:
      break;
    case SDLK_RIGHT:case SDLK_SPACE:
      break;
    case SDLK_ESCAPE:
      break;
  }
  } else {
  if (ev.key.keysym.mod & KMOD_CTRL) {
  switch (ev.key.keysym.sym) { // CTRL + sym
    case SDLK_n:
      new_slide(prog);
      break;
    case SDLK_DELETE:
      delete_slide(prog);
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
    case SDLK_BACKSPACE:
      if (strlen(ACTIVE_TEXT_BOX(*prog).text) != 0) {
        int index = strlen(ACTIVE_TEXT_BOX(*prog).text) - 1;
        if (ACTIVE_TEXT_BOX(*prog).text[index] & 0x80) {
          index++;
          while ((ACTIVE_TEXT_BOX(*prog).text[--index] & 0xC0) != 0xC0);
        }
        ACTIVE_TEXT_BOX(*prog).text[index] = '\0';
      }
      break;
  }
  }
  }
  } else if (ev.type == SDL_TEXTINPUT) {
    strcat(ACTIVE_TEXT_BOX(*prog).text, ev.text.text);
  }
  }
}

