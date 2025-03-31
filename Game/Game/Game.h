#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SIZE = 50;
const int PLAYER_SPEED = 5;
const int GRAVITY = 1;
const int MAX_JUMP = 25;  // Lực nhảy tối đa
const int MIN_JUMP = 10;  // Lực nhảy tối thiểu
class Platform {
public:
    int x, y, w, h;
    Platform(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

    void render(SDL_Renderer* renderer) {
        SDL_Rect rect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Màu xanh
        SDL_RenderFillRect(renderer, &rect);
    }
};
class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void close();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    int playerX, playerY;
    int playerVelX, playerVelY;
    bool isJumping;
    bool jumpKeyHeld;
    int jumpCharge;  // Lực nhảy tích lũy
    std::vector<Platform> platforms;
    Uint32 jumpStartTime;
    void handleEvents();
    void update();
    void render();
    void checkCollisions(); // Kiểm tra va chạm với platform
};

#endif
