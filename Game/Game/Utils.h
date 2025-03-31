#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>

bool init(SDL_Window*& window, SDL_Renderer*& renderer);
void close(SDL_Window*& window, SDL_Renderer*& renderer);

#endif
