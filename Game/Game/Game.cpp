#include "Game.h"
#include "Platform.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Utils.h"


Game::Game() : isRunning(false), window(nullptr), renderer(nullptr) ,facing(0) {
    // Đặt nhân vật bắt đầu ở dưới đáy của map
    playerX = SCREEN_WIDTH / 2;
    playerY = MAP_HEIGHT - PLAYER_SIZE;
    playerVelX = 0;
    playerVelY = 0;
    isJumping = false;
    jumpKeyHeld = false;
    jumpCharge = MIN_JUMP;
    jumpStartTime = 0;
    camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    currentFrame = 0;
    lastFrameTime = 0;
    playerTexture = nullptr;
    // Tạo các platform (chỉnh vị trí sao cho phù hợp với map dọc)
    platforms.push_back(Platform(300, 450, 200, 20));
    platforms.push_back(Platform(100, 2200, 150, 20));
    platforms.push_back(Platform(500, 2000, 180, 20));
    platforms.push_back(Platform(700, 1500, 150, 20));
    platforms.push_back(Platform(400, 1300, 180, 20));
    platforms.push_back(Platform(150, 1000, 250, 20));
    platforms.push_back(Platform(600, 700, 220, 20));
}

Game::~Game() { close(); }

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize!\n";
        return false;
    }
    window = SDL_CreateWindow("Jump Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;
    SDL_Surface* tempSurface = IMG_Load("C:/Users/LAPTOP/OneDrive - vnu.edu.vn/Desktop/Game/Game/assets/player.png");
    if (!tempSurface) {
        std::cout << "Failed to load player image: " << IMG_GetError() << std::endl;
        return false;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Khởi tạo frame animation
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();

    isRunning = true;
    return true;
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                // Nếu đang nhảy và chưa bắt đầu rơi (ascend) thì không điều chỉnh hướng
                if (isJumping) {
                    if (playerVelY > 0) { // Nếu đang rơi xuống, cho điều khiển với tốc độ bằng 1/2
                        playerVelX = -PLAYER_SPEED / 2;
                    }
                    // Nếu đang nhảy lên (ascend), không thay đổi playerVelX
                }
                else { // Không nhảy, hoạt động bình thường
                    if (!jumpKeyHeld)
                        playerVelX = -PLAYER_SPEED;
                }
                // Chỉ cập nhật hướng khi được phép điều khiển (trong rơi hoặc không nhảy)
                if (!isJumping || (isJumping && playerVelY > 0))
                    facing = -1;
                break;

            case SDLK_RIGHT:
            case SDLK_d:
                if (isJumping) {
                    if (playerVelY > 0) { // Nếu đang rơi xuống, điều khiển với tốc độ 1/2
                        playerVelX = PLAYER_SPEED / 2;
                    }
                    // Nếu đang nhảy lên, không điều chỉnh
                }
                else {
                    if (!jumpKeyHeld)
                        playerVelX = PLAYER_SPEED;
                }
                if (!isJumping || (isJumping && playerVelY > 0))
                    facing = 1;
                break;

            case SDLK_SPACE:
                if (!isJumping && !jumpKeyHeld) {
                    jumpKeyHeld = true;
                    jumpCharge = MIN_JUMP;
                    jumpStartTime = SDL_GetTicks();
                    // Khi bắt đầu giữ nhảy, tắt vận tốc ngang để không di chuyển
                    playerVelX = 0;
                }
                break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                if (jumpKeyHeld && !isJumping) {
                    Uint32 holdTime = SDL_GetTicks() - jumpStartTime;
                    jumpCharge = MIN_JUMP + (holdTime / 50);
                    if (jumpCharge > MAX_JUMP) jumpCharge = MAX_JUMP;
                    playerVelY = -jumpCharge; // Lực nhảy dọc
                    // Áp dụng vận tốc ngang theo hướng đã lưu chỉ nếu đang rơi (nếu không, giữ hướng 0)
                    if (!isJumping && facing != 0) {
                        // Nếu trong lúc nhảy, ta không cho điều khiển hướng; 
                        // khi nhả SPACE, nếu đang ở trạng thái đứng (không nhảy) thì sẽ không có vận tốc ngang
                        // Nếu đã có hướng được cập nhật từ keydown trong lúc rơi, ta sử dụng hướng đó:
                        playerVelX = facing * PLAYER_SPEED;
                    }
                    isJumping = true;
                }
                jumpKeyHeld = false;
            }
            else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a ||
                e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
                // Nếu không đang giữ nhảy, reset vận tốc ngang
                if (!jumpKeyHeld && !isJumping)
                    playerVelX = 0;
            }
        }
    }
}



// Cập nhật va chạm theo trục Y và X riêng biệt
void Game::checkCollisions() {
    // --- Xử lý va chạm theo trục Y (vertical collisions) ---
    int newY = playerY + playerVelY;
    for (Platform& platform : platforms) {
        SDL_Rect playerRect = { playerX, newY, PLAYER_SIZE, PLAYER_SIZE };
        SDL_Rect platformRect = { platform.x, platform.y, platform.w, platform.h };

        if (SDL_HasIntersection(&playerRect, &platformRect)) {
            if (playerVelY > 0) { // Đang rơi xuống
                // Khi nhân vật tiếp đất, đặt nó ngay trên platform và reset vận tốc ngang
                newY = platform.y - PLAYER_SIZE;
                playerVelY = 0;
                isJumping = false;
                playerVelX = 0;  // RESET horizontal velocity khi tiếp đất
            }
            else if (playerVelY < 0) { // Đang nhảy lên
                // Nếu va chạm khi nhảy lên (chạm đầu)
                newY = platform.y + platform.h + 1; // +1 để tránh kẹt
                playerVelY = 0; // Reset tốc độ Y
            }
        }
    }
    playerY = newY;

    // --- Xử lý va chạm theo trục X (horizontal collisions) ---
    int newX = playerX + playerVelX;
    for (Platform& platform : platforms) {
        SDL_Rect playerRect = { newX, playerY, PLAYER_SIZE, PLAYER_SIZE };
        SDL_Rect platformRect = { platform.x, platform.y, platform.w, platform.h };

        if (SDL_HasIntersection(&playerRect, &platformRect)) {
            if (playerVelX > 0) { // Di chuyển sang phải
                newX = platform.x - PLAYER_SIZE;
            }
            else if (playerVelX < 0) { // Di chuyển sang trái
                newX = platform.x + platform.w;
            }
            playerVelX = 0;
        }
    }
    playerX = newX;
}


void Game::update() {
    // --- Giảm tốc độ rơi (nhảy rơi chậm hơn) ---
    const int MAX_FALL_SPEED = 10; // Giảm so với 20 để rơi chậm hơn

    // Cập nhật vị trí theo vận tốc đã tính (trước khi cộng thêm trọng lực)
    playerX += playerVelX;
    playerY += playerVelY;

    // Cập nhật camera dựa trên vị trí nhân vật
    camera.x = playerX - SCREEN_WIDTH / 2;
    camera.y = playerY - SCREEN_HEIGHT / 2;
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > MAP_WIDTH - SCREEN_WIDTH) camera.x = MAP_WIDTH - SCREEN_WIDTH;
    if (camera.y > MAP_HEIGHT - SCREEN_HEIGHT) camera.y = MAP_HEIGHT - SCREEN_HEIGHT;

    // Áp dụng trọng lực (với MAX_FALL_SPEED thấp hơn)
    playerVelY += GRAVITY;
    if (playerVelY > MAX_FALL_SPEED)
        playerVelY = MAX_FALL_SPEED;

    checkCollisions();
    if (playerY >= MAP_HEIGHT - PLAYER_SIZE) {
        playerY = MAP_HEIGHT - PLAYER_SIZE;
        playerVelY = 0;
        isJumping = false;
        playerVelX = 0; // RESET horizontal velocity khi chạm đất
    }
    // Giới hạn nhân vật trong bản đồ
    if (playerX < 0) playerX = 0;
    if (playerX > MAP_WIDTH - PLAYER_SIZE) playerX = MAP_WIDTH - PLAYER_SIZE;
    if (playerY < 0) playerY = 0;
    if (playerY > MAP_HEIGHT - PLAYER_SIZE) {
        playerY = MAP_HEIGHT - PLAYER_SIZE;
        playerVelY = 0;
        isJumping = false;
        // --- Cập nhật animation cho nhân vật ---
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime >= FRAME_DURATION) {
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
            lastFrameTime = now;
        }
    }
}

void Game::render() {
    // Debug in ra vị trí
    std::cout << "Player Pos: (" << playerX << ", " << playerY << ")\n";
    std::cout << "Camera Pos: (" << camera.x << ", " << camera.y << ")\n";

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ các platform
    for (Platform& platform : platforms)
        platform.render(renderer, camera);

    // Vẽ nhân vật
    SDL_Rect srcRect = { currentFrame * PLAYER_SIZE, 0, PLAYER_SIZE, PLAYER_SIZE };
    SDL_Rect dstRect = { playerX - camera.x, playerY - camera.y, PLAYER_SIZE, PLAYER_SIZE };
    SDL_RenderCopy(renderer, playerTexture, &srcRect, &dstRect);

    SDL_RenderPresent(renderer);
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::close() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}