#pragma once
#include "CommonFun.h"

class BaseObject
{

public:
    BaseObject();
    //~BaseObject();

    void SetRect(const int x, const int y)
    {
        rect.x = x;
        rect.y = y;
    }

    SDL_Rect GetRect()
    {
        return rect;
    }

    SDL_Texture* GetTexture()
    {
        return p_object;
    }

    void LoadImage(string path, SDL_Renderer* renderer);
    void Render(SDL_Renderer* des, SDL_Rect* clip);


protected:
    SDL_Texture* p_object;
    SDL_Rect rect;
};
BaseObject::BaseObject()
{
    p_object = nullptr;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
}


void BaseObject::LoadImage(string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
        cout << "Unable to load image " << path << " SDL_image Error: "
        << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        {
            rect.w = loadedSurface->w;
            rect.h = loadedSurface->h;
        }

        if (newTexture == nullptr)
            cout << "Unable to create texture from " << path << " SDL Error: "
            << SDL_GetError() << endl;
        SDL_FreeSurface(loadedSurface);
    }


    p_object = newTexture;

}


void BaseObject::Render(SDL_Renderer* des, SDL_Rect* clip)
{
    SDL_Rect renderquad = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderCopy(des, p_object, clip, &renderquad);
}