#include <SDL3/SDL.h>

#define WIDTH 900
#define HEIGHT 600

int main (int argc, char **argv) {
  int app_running = 1;
  SDL_Event e;
  SDL_Window *pwindow = SDL_CreateWindow("Random walk", WIDTH, HEIGHT, 0);
  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  while(app_running) {
    while(SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_EVENT_QUIT: {
          app_running = 0;
          break;
        }
      }
    }
  }
  return 0;
}
