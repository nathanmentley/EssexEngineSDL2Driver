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

#include <EssexEngineSDL2Driver/SDL2Driver.h>

using EssexEngine::Context;
using EssexEngine::WeakPointer;

using EssexEngine::Drivers::SDL2::SDL2Driver;
using EssexEngine::Daemons::Gfx::IGfxDriver;
using EssexEngine::Daemons::Sfx::ISfxDriver;

extern "C" {
    void driver_init(WeakPointer<Context> context) {
        WeakPointer<SDL2Driver> driver = (new SDL2Driver(context));
        
        context->RegisterDriver<IGfxDriver>(
            driver.Cast<IGfxDriver>() 
        );
        context->RegisterDriver<ISfxDriver>(
            driver.Cast<ISfxDriver>() 
        );
    }
}
