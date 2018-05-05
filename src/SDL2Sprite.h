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
#pragma once

#include <SDL2/SDL.h>

#include <EssexEngineCore/CachedPointer.h>
#include <EssexEngineGfxDaemon/IGfxDriver.h>
#include <EssexEngineFileSystemDaemon/IFileBuffer.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Sprite:public Daemons::Gfx::ISprite {
    public:
        SDL2Sprite(
            CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> _fileBuffer, WeakPointer<SDL_Texture> _texture, int _x, int _y, int _width, int _height
        ): fileBuffer(std::move(_fileBuffer)) {
            texture = _texture;
            x = _x;
            y = _y;
            width = _width;
            height = _height;
        }
        ~SDL2Sprite() {}
        
        SDL_Texture* GetSprite() {
            return texture;
        }

        int GetX() {
            return x;
        }
        int GetY() {
            return y;
        }
        int GetWidth() {
            return width;
        }
        int GetHeight() {
            return height;
        }
        int GetTotalWidth() {
            int w, h;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            return w;
        }
        int GetTotalHeight() {
            int w, h;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            return h;
        }
    private:
        CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileBuffer;
        WeakPointer<SDL_Texture> texture;
        int x;
        int y;
        int width;
        int height;
    };
}}};
