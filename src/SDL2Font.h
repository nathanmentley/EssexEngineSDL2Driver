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
#include <SDL2/SDL_ttf.h>

#include <EssexEngineCore/CachedPointer.h>
#include <EssexEngineCore/BaseUniquePointer.h>

#include <EssexEngineGfxDaemon/IFont.h>
#include <EssexEngineFileSystemDaemon/IFileBuffer.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Font:public Daemons::Gfx::ISprite {
    public:
        SDL2Font(CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileBuffer, int _fontSize):
            font(
                BaseUniquePointer<TTF_Font>(
                    
                    TTF_OpenFontRW(
                        SDL_RWFromMem(fileBuffer->GetBuffer(), fileBuffer->GetSize()),
                        1,
                        _fontSize
                    ),
                    SDL2Font::DeleteFont
                )
            )
        {
            fontSize = _fontSize;
        }
        ~SDL2Font() {}

        WeakPointer<TTF_Font> GetFont() {
            return font.ToWeakPointer();
        }

        int GetFontSize() {
            return fontSize;
        }

        static void DeleteFont(TTF_Font* ptr) {
            TTF_CloseFont(ptr);
        }
    private:
        BaseUniquePointer<TTF_Font> font;
        int fontSize;
    };
}}};
