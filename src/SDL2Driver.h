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

#include <string>
#include <map>
#include <utility>

#include <OpenGL/gl.h>

#include <EssexEngineCore/WeakPointer.h>
#include <EssexEngineCore/CachedPointer.h>
#include <EssexEngineCore/UniquePointer.h>
#include <EssexEngineCore/BaseDriver.h>
#include <EssexEngineCore/Context.h>
#include <EssexEngineCore/LogDaemon.h>
#include <EssexEngineGfxDaemon/IGfxDriver.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Driver:public Core::Drivers::Base::BaseDriver, public Daemons::Gfx::IGfxDriver
    {
    public:
        SDL2Driver(WeakPointer<Context> _context);
        ~SDL2Driver();
        
        //IDriver
        void Init() {
            if(GetContext()->HasDaemon<Core::Logging::LogDaemon>()) {
                GetContext()->GetDaemon<Core::Logging::LogDaemon>()->LogLine(
                    "Loading Driver [%s] [%s]",
                    GetDriverName().c_str(),
                    GetDriverVersion().c_str()
                );
            }
        }
        
        //IGfxDriver
        void Setup(WeakPointer<Daemons::Window::IRenderContext> target);

        void StartRender(WeakPointer<Daemons::Window::IRenderContext> target);
        void FinishRender(WeakPointer<Daemons::Window::IRenderContext> target);

        void RenderEntity(WeakPointer<Daemons::Window::IRenderContext> target, WeakPointer<Daemons::Gfx::Entity> entity);
        void RenderModel(WeakPointer<Daemons::Window::IRenderContext> target, WeakPointer<Daemons::Gfx::Model> model);
        void RenderString(WeakPointer<Daemons::Window::IRenderContext> target, std::string data, int x, int y);

        WeakPointer<Daemons::Gfx::ISprite> GetSprite(CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileContent, int _x, int _y, int _width, int _height);
        
        //BaseDriver
        std::string GetDriverName() { return "SDL2"; }
        std::string GetDriverVersion() { return ESSEX_ENGINE_VERSION; }
    private:
    };
}}};
