#include <SDL.h>
#include <iostream>
#include "Menu.h"
#include "Utils.h"
#include "Game.h"
#include <SDL_image.h>
int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    int selectedLevel = showMenu(renderer);
    if (selectedLevel > 0) {
        std::cout << "Selected Level: " << selectedLevel << std::endl;
    }
    else {
        std::cout << "Game exited!" << std::endl;
    }
    Game game;
    if (game.init()) {
        game.run();
    }
    close(window, renderer);
    return 0;
}
