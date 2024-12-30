#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "temp_vars.h"
#include "main.h"

#define ACTIVE_TEXT_BOX(prog) (prog).current_slide->text_boxes[(prog).current_slide->active_box_index]

int main(int argc, char** argv) {
  // Will add error handlers for these later
  (void)SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Presento", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  (void)TTF_Init();
  TTF_Font *regular_font = TTF_OpenFont("fonts/calibri-regular.ttf", 20);
  TTF_Font *bold_font = TTF_OpenFont("fonts/calibri-bold.ttf", 20);

  const SDL_Rect edit_slide_rect = {EDIT_SLIDE_BOX_X, EDIT_SLIDE_BOX_Y, EDIT_SLIDE_BOX_W, EDIT_SLIDE_BOX_H};
  
  Program program = {NULL, (Slide *)malloc(sizeof *program.first_slide), 1, false};
  setup_slide(program.first_slide, 0, NULL);
  program.current_slide = program.first_slide;

  Uint64 start_time, delta_time;
  const int mspf = 16; // 62.5 fps
  while (!program.should_quit) {
    start_time = SDL_GetTicks64();
    
    handle_events(&program);

    SDL_SetRenderDrawColor(renderer, 255, 240, 215, SDL_ALPHA_OPAQUE); // will probably change this colour later
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &edit_slide_rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    if (program.current_slide->text_box_count != 0) {
      SDL_Rect text_box_rect = ACTIVE_TEXT_BOX(program).dimensions;
      text_box_rect.x += EDIT_SLIDE_BOX_X;
      text_box_rect.y += EDIT_SLIDE_BOX_Y;
      SDL_RenderDrawRect(renderer, &text_box_rect);
    }

    SDL_RenderPresent(renderer);

    delta_time = SDL_GetTicks64() - start_time;
    if (mspf > delta_time)
      SDL_Delay(mspf - delta_time);
    delta_time = SDL_GetTicks64() - start_time;
  }

  free_program(&program);

  TTF_CloseFont(bold_font);
  TTF_CloseFont(regular_font);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void handle_events(Program *prog) {
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    switch (ev.type) {
      case SDL_QUIT:
        prog->should_quit = true;
        break;
      case SDL_KEYDOWN:
        if (ev.key.keysym.mod & KMOD_CTRL) {
          switch (ev.key.keysym.sym) { // CTRL + sym
            case SDLK_n: // new slide
              {
                Slide *next = prog->current_slide->next_slide;
                prog->current_slide->next_slide = (Slide *)malloc(sizeof *prog->current_slide->next_slide);
                setup_slide(prog->current_slide->next_slide, prog->current_slide->index + 1, next);
                prog->current_slide = prog->current_slide->next_slide;
                while (next != NULL) {
                  next->index++;
                  next = next->next_slide;
                }
              }
              break;
            case SDLK_DELETE: // delete slide
              if (prog->current_slide->index == 0 && prog->current_slide->next_slide != NULL) {
                prog->first_slide = prog->current_slide->next_slide;
                free(prog->current_slide->text_boxes);
                free(prog->current_slide);
                Slide *dec_index = prog->first_slide;
                while (dec_index != NULL) {
                  dec_index->index--;
                  dec_index = dec_index->next_slide;
                }
                prog->current_slide = prog->first_slide;
              } else if (prog->current_slide != 0) {
                Slide *prev = prog->first_slide;
                while (prev->index != prog->current_slide->index - 1)
                  prev = prev->next_slide;
                prev->next_slide = prog->current_slide->next_slide;
                free(prog->current_slide->text_boxes);
                free(prog->current_slide);
                prog->current_slide = (prev->next_slide == NULL) ? prev : prev->next_slide;
                prev = prev->next_slide;
                while (prev != NULL) {
                  prev->index--;
                  prev = prev->next_slide;
                }
              }
              break;
          }
        }
        switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE: // quit (probably remove)
            push_quit();
            break;
          case SDLK_TAB: // switch active text box
            prog->current_slide->active_box_index = (prog->current_slide->active_box_index + 1) % prog->current_slide->text_box_count;
            break;
          case SDLK_PAGEUP: // prev slide
            if (prog->current_slide->index != 0) {
              int prev_slide_index = prog->current_slide->index - 1;
              prog->current_slide = prog->first_slide;
              while (prog->current_slide->index != prev_slide_index)
                prog->current_slide = prog->current_slide->next_slide;
            }
            break;
          case SDLK_PAGEDOWN: // next slide
            if (prog->current_slide->next_slide != NULL)
              prog->current_slide = prog->current_slide->next_slide;
            break;
          case SDLK_UP: // move active text box up
            if (prog->current_slide->text_box_count != 0)
              ACTIVE_TEXT_BOX(*prog).dimensions.y -= (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
            if (ACTIVE_TEXT_BOX(*prog).dimensions.y < 0)
              ACTIVE_TEXT_BOX(*prog).dimensions.y = 0;
            break;
          case SDLK_DOWN: // move active text box down
            if (prog->current_slide->text_box_count != 0)
              ACTIVE_TEXT_BOX(*prog).dimensions.y += (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
            if (ACTIVE_TEXT_BOX(*prog).dimensions.y + ACTIVE_TEXT_BOX(*prog).dimensions.h > EDIT_SLIDE_BOX_H)
              ACTIVE_TEXT_BOX(*prog).dimensions.y = EDIT_SLIDE_BOX_H - ACTIVE_TEXT_BOX(*prog).dimensions.h;
            break;
          case SDLK_LEFT: // move active text box left
            if (prog->current_slide->text_box_count != 0)
              ACTIVE_TEXT_BOX(*prog).dimensions.x -= (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
            if (ACTIVE_TEXT_BOX(*prog).dimensions.x < 0)
              ACTIVE_TEXT_BOX(*prog).dimensions.x = 0;
            break;
          case SDLK_RIGHT: // move active text box right
            if (prog->current_slide->text_box_count != 0)
              ACTIVE_TEXT_BOX(*prog).dimensions.x += (SDL_GetModState() & KMOD_SHIFT) ? 10 : 1;
            if (ACTIVE_TEXT_BOX(*prog).dimensions.x + ACTIVE_TEXT_BOX(*prog).dimensions.w > EDIT_SLIDE_BOX_W)
              ACTIVE_TEXT_BOX(*prog).dimensions.x = EDIT_SLIDE_BOX_W - ACTIVE_TEXT_BOX(*prog).dimensions.w;
            break;
        }
    }
  }
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