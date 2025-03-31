#include "Menu.h"
#include <SDL.h>

int showMenu(SDL_Renderer* renderer) {
    bool quit = false;
    SDL_Event e;
    int selectedLevel = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return -1;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_1:
                    selectedLevel = 1;
                    quit = true;
                    break;
                case SDLK_2:
                    selectedLevel = 2;
                    quit = true;
                    break;
                case SDLK_3:
                    selectedLevel = 3;
                    quit = true;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Placeholder UI rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect box1 = { 200, 150, 400, 50 };
        SDL_Rect box2 = { 200, 250, 400, 50 };
        SDL_Rect box3 = { 200, 350, 400, 50 };
        SDL_RenderFillRect(renderer, &box1);
        SDL_RenderFillRect(renderer, &box2);
        SDL_RenderFillRect(renderer, &box3);

        SDL_RenderPresent(renderer);
    }
    return selectedLevel;
}
