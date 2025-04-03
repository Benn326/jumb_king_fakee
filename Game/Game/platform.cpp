#include "platform.h"
#include <iostream>
Platform::Platform(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

void Platform::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    std::cout << "Rendering platform at: " << x << ", " << y << std::endl;
    SDL_Rect rect = { x - camera.x, y - camera.y, w, h };
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}