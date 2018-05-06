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

#include <EssexEngineSfxDaemon/IAudio.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Audio:public Daemons::Sfx::IAudio {
        public:
            SDL2Audio(
                WeakPointer<Daemons::FileSystem::IFileBuffer> data, int _x, int _y, int _z
            ): audio(
                BaseUniquePointer<Mix_Chunk>(SDL2Audio::DeleteMixChunk)
            ) {
                audio.Replace(
                    Mix_LoadWAV_RW(
                        SDL_RWFromMem(data->GetBuffer(), data->GetSize()),
                        1
                    )
                );
                
                SetChannel(-1);
                SetPosition(_x, _y, _z);
            }

            WeakPointer<Mix_Chunk> GetAudio() {
                return audio.ToWeakPointer();
            }
            
            int GetX() {
                return x;
            }

            int GetY() {
                return y;
            }

            int GetZ() {
                return z;
            }

            int GetChannel() {
                return channel;
            }

            void SetChannel(int _channel) {
                channel = _channel;
            }

            void SetPosition(int _x, int _y, int _z) {
                x = _x;
                y = _y;
                z = _z;
            }

            static void DeleteMixChunk(Mix_Chunk* ptr) { Mix_FreeChunk(ptr); }
        private:
            BaseUniquePointer<Mix_Chunk> audio;
            int x;
            int y;
            int z;
            int channel;
    };
}}};
