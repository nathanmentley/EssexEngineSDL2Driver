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

#include <EssexEngineSDL2Driver/SDL2Driver.h>

extern "C" {
    void driver_init(EssexEngine::Context* context) {
        EssexEngine::Drivers::SDL2::SDL2Driver* sdl2Driver = new EssexEngine::Drivers::SDL2::SDL2Driver(context);
        
        context->RegisterDriver<EssexEngine::Daemons::System::ISystemDriver>(sdl2Driver);
        context->RegisterDriver<EssexEngine::Daemons::Input::IInputDriver>(sdl2Driver);
        context->RegisterDriver<EssexEngine::Daemons::Gfx::IGfxDriver>(sdl2Driver);
    }
}