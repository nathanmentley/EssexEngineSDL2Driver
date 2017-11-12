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

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <EssexEngineSDL2Driver/SDL2ImGuiBridge.h>

#include <EssexEngineCore/WeakPointer.h>
#include <EssexEngineCore/CachedPointer.h>
#include <EssexEngineCore/BaseDriver.h>
#include <EssexEngineCore/Context.h>
#include <EssexEngineCore/LogDaemon.h>
#include <EssexEngineGfxDaemon/IGfxDriver.h>
#include <EssexEngineInputDaemon/IInputDriver.h>
#include <EssexEngineSystemDaemon/ISystemDriver.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Driver:public Core::Drivers::Base::BaseDriver, public Daemons::Gfx::IGfxDriver, public Daemons::Input::IInputDriver, public Daemons::System::ISystemDriver
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
        void Setup();
        
        int GetScreenWidth();
        int GetScreenHeight();
        
        void StartRender();
        void FinishRender();
        
        void RenderEntity(WeakPointer<Daemons::Gfx::Entity> entity);
        void RenderModel(WeakPointer<Daemons::Gfx::Model> model);
        void RenderString(std::string data, int x, int y);
        
        WeakPointer<Daemons::Gfx::ISprite> GetSprite(CachedPointer<Daemons::FileSystem::IFileBuffer> fileContent, int _x, int _y, int _width, int _height);
        
        //IInputDriver
        bool IsKeyPressed(Daemons::Input::KeyboardButton::InputKeys key);
        bool IsMousePressed(Daemons::Input::MouseButton::MouseButtons key,  Daemons::Input::MouseEventLocation &data);
        
        //ISystemDriver
        void StartTimer();
        int GetElapsedTime();
        void Sleep(int milliseconds);
        
        //BaseDriver
        std::string GetDriverName() { return "SDL2"; }
        std::string GetDriverVersion() { return ESSEX_ENGINE_VERSION; }
    private:
        std::map<std::string, SDL_Texture*> textureCache;
        SDL_Window* window;
        SDL_Renderer* renderer;
        Uint32 lastTicks;
    };
}}};
