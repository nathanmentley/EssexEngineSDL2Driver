/* 
 * Essex Engine
 * 
 * Copyright (C) 2017 Nathan Mentley - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BSD license.
 *
 * You should have received a copy of the BSD license with
 * this file. If not, please visit: https://github.com/nathanmentley/EssexEngine
 */

#include <EssexEngineSDL2Driver/SDL2Sprite.h>

EssexEngine::Drivers::SDL2::SDL2Sprite::SDL2Sprite(SDL_Texture* _texture, int _x, int _y, int _width, int _height) {
    texture = _texture;
    x = _x;
    y = _y;
    width = _width;
    height = _height;
}

EssexEngine::Drivers::SDL2::SDL2Sprite::~SDL2Sprite() {
}

SDL_Texture* EssexEngine::Drivers::SDL2::SDL2Sprite::GetSprite() {
    return texture;
}

int EssexEngine::Drivers::SDL2::SDL2Sprite::GetX() {
    return x;
}

int EssexEngine::Drivers::SDL2::SDL2Sprite::GetY() {
    return y;
}

int EssexEngine::Drivers::SDL2::SDL2Sprite::GetWidth() {
    return width;
}

int EssexEngine::Drivers::SDL2::SDL2Sprite::GetHeight() {
    return height;
}

int EssexEngine::Drivers::SDL2::SDL2Sprite::GetTotalWidth() {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    return w;
}

int EssexEngine::Drivers::SDL2::SDL2Sprite::GetTotalHeight() {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    return h;
}
