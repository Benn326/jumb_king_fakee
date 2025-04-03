#pragma once
#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL.h>

class Platform {
public:
    int x, y, w, h;
    Platform(int x, int y, int w, int h);
    void render(SDL_Renderer* renderer, const SDL_Rect& camera);
};

#endif
