#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "temp_vars.h"
#include "main.h"

// move somewhere else (inline?)
void setup_default_boxes(TextBox *text_boxes) {
  text_boxes[0] = (TextBox){{40 + EDIT_SLIDE_BOX_X, 40 + EDIT_SLIDE_BOX_Y, 720, 80}, {0}, true};
  text_boxes[1] = (TextBox){{40 + EDIT_SLIDE_BOX_X, 160 + EDIT_SLIDE_BOX_Y, 720, 250}, {0}, false};
}

int main(int argc, char** argv) {
  // Will add error handlers for these later
  (void)SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Presento", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  (void)TTF_Init();
  TTF_Font *regular_font = TTF_OpenFont("fonts/calibri-regular.ttf", 20);
  TTF_Font *bold_font = TTF_OpenFont("fonts/calibri-bold.ttf", 20);

  const SDL_Rect edit_slide_rect = {EDIT_SLIDE_BOX_X, EDIT_SLIDE_BOX_Y, EDIT_SLIDE_BOX_W, EDIT_SLIDE_BOX_H};
  
  Slide first_slide = {(TextBox *)malloc(8*sizeof(TextBox)), 0, 2, 1, NULL};
  setup_default_boxes(first_slide.text_boxes);
  Program program = {&first_slide, &first_slide, 1, false};
  while (!program.should_quit) {
    SDL_SetRenderDrawColor(renderer, 255, 240, 215, SDL_ALPHA_OPAQUE); // will probably change this colour later
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &edit_slide_rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &program.current_slide->text_boxes[program.current_slide->active_box_index].dimensions);

    handle_events(&program);

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 fps (doesn't really matter)
  }

  free(first_slide.text_boxes);

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
        switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE:
            push_quit();
            break;
          case SDLK_TAB:
            prog->current_slide->active_box_index = (prog->current_slide->active_box_index + 1) % prog->current_slide->text_box_count;
        }
      
    }
  }
}
