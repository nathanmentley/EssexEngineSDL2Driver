/* 
 * Essex Engine
 * 
 * Copyright (C) 2018 Nathan Mentley - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BSD license.
 *
 * You should have received a copy of the BSD license with
 * this file. If not, please visit: https://github.com/nathanmentley/EssexEngine
 */
#pragma once

#include <EssexEngineSfxDaemon/IMusic.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Music:public Daemons::Sfx::IMusic {
        public:
            SDL2Music(WeakPointer<Daemons::FileSystem::IFileBuffer> data): music(
                BaseUniquePointer<Mix_Music>(SDL2Music::DeleteMixMusic)
            ) {
                music.Replace(
                    Mix_LoadMUS_RW(
                        SDL_RWFromMem(data->GetBuffer(), data->GetSize()),
                        1
                    )
                );
            }

            WeakPointer<Mix_Music> GetMusic() {
                return music.ToWeakPointer();
            }

            static void DeleteMixMusic(Mix_Music* ptr) { Mix_FreeMusic(ptr); }
        private:
            BaseUniquePointer<Mix_Music> music;
    };
}}};
