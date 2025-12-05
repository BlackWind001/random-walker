#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#define WIDTH 900
#define HEIGHT 600

#define RECT_HEIGHT 5
#define RECT_WIDTH 5
#define SCALE 10

enum Direction : int {
  UP, DOWN, LEFT, RIGHT
};

typedef struct {
    Uint8 r, g, b;
} RGB;


class Walker {
  public:
    Walker (SDL_Surface *psurface) {
      pwindow_surface = psurface;
      rect = (SDL_Rect)  {WIDTH/2, HEIGHT/2, RECT_HEIGHT, RECT_WIDTH};

      populate_color();
    }

    // For initialization in manager
    Walker () {}

    void move () {
      if (pwindow_surface == nullptr) {
        return;
      }
      move_rect(&rect, get_direction());
    }

  private:

    SDL_Rect rect;
    SDL_Surface *pwindow_surface = nullptr;
    Uint32 color;

    Direction get_direction () {
      srand(rand());

      return (Direction)(rand() % 4);
    }

    RGB hsl_to_rgb(float h, float s, float l) {
        RGB rgb;
        
        if (s == 0) {
            // Achromatic: gray
            rgb.r = rgb.g = rgb.b = (unsigned char)(l * 255);
            return rgb;
        }
        
        float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        float hk = h / 360.0f;
        
        float t[3] = {hk + 1.0f/3.0f, hk, hk - 1.0f/3.0f};
        
        for (int i = 0; i < 3; i++) {
            if (t[i] < 0) t[i] += 1.0f;
            if (t[i] > 1) t[i] -= 1.0f;
            
            if (t[i] * 6 < 1) {
                t[i] = p + (q - p) * 6 * t[i];
            } else if (t[i] * 2 < 1) {
                t[i] = q;
            } else if (t[i] * 3 < 2) {
                t[i] = p + (q - p) * ((2.0f/3.0f) - t[i]) * 6;
            } else {
                t[i] = p;
            }
        }
        
        rgb.r = (Uint8)(t[0] * 255);
        rgb.g = (Uint8)(t[1] * 255);
        rgb.b = (Uint8)(t[2] * 255);
        
        return rgb;
    }

    void populate_color () {
      srand(time(NULL));

      float h = (rand() % 360);
      float s = 1.0;
      float l = 0.5;

      RGB temp = hsl_to_rgb(h, s, l);
      color = SDL_MapRGB(
        SDL_GetPixelFormatDetails(pwindow_surface->format),
        NULL,
        temp.r,
        temp.g,
        temp.b
      );
    }

    void move_rect (SDL_Rect *rect, Direction dir) {
      if (pwindow_surface == nullptr) {
        return;
      }
      for (int i = 0; i < SCALE; i++) {
        switch (dir) {
          case UP: {
            rect->y -= 1;
            break;
          }
          case DOWN: {
            rect->y += 1;
            break;
          }
          case LEFT: {
            rect->x -= 1;
            break;
          }
          default: {
            rect->x += 1;
            break;
          }
        }
        SDL_FillSurfaceRect(pwindow_surface, rect, color);
      }
    }
};

class WalkersManager {
  public:
    WalkersManager (int num, SDL_Surface *psurface) {
      num_walkers = num;
      walkers = new Walker[num_walkers];

      for(int i = 0; i < num_walkers; i++) {
        walkers[i] = Walker(psurface);  // Copy construction
      }
    }

    ~WalkersManager () {
      delete[] walkers;
    }

  private:
    int num_walkers;
    Walker* walkers;
};

int main (int argc, char **argv) {
  int app_running = 1;
  SDL_Event e;
  SDL_Window *pwindow = SDL_CreateWindow("Random walk", WIDTH, HEIGHT, 0);
  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  Walker *walker = new Walker(psurface);

  while(app_running) {
    while(SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_EVENT_QUIT: {
          app_running = 0;
          break;
        }
      }
    }
    

     // Decide a random direction here.
    walker->move();
    SDL_UpdateWindowSurface(pwindow);
    SDL_Delay(10);
  }
  return 0;
}
