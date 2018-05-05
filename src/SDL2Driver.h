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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <EssexEngineCore/WeakPointer.h>
#include <EssexEngineCore/CachedPointer.h>
#include <EssexEngineCore/UniquePointer.h>
#include <EssexEngineCore/BaseDriver.h>
#include <EssexEngineCore/Context.h>
#include <EssexEngineCore/LogDaemon.h>

#include <EssexEngineInputDaemon/IInputDriver.h>
#include <EssexEngineGfxDaemon/IGfxDriver.h>
#include <EssexEngineSfxDaemon/ISfxDriver.h>

#include <EssexEngineSDL2Driver/SDL2Sprite.h>
#include <EssexEngineSDL2Driver/SDL2Music.h>
#include <EssexEngineSDL2Driver/SDL2Audio.h>

namespace EssexEngine{
namespace Drivers{
namespace SDL2{
    class SDL2Driver:public Core::Drivers::Base::BaseDriver, public Daemons::Gfx::IGfxDriver, public Daemons::Sfx::ISfxDriver, public Daemons::Input::IInputDriver
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

        WeakPointer<Daemons::Gfx::ISprite> GetSprite(WeakPointer<Daemons::Window::IRenderContext> target, CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileContent, int _x, int _y, int _width, int _height);
        
        //IInputDriver
        bool IsKeyPressed(Daemons::Input::KeyboardButton::InputKeys key);
        bool IsMousePressed(Daemons::Input::MouseButton::MouseButtons key,  Daemons::Input::MouseEventLocation &data);

        //ISfxDriver
        void PlayAudio(WeakPointer<Daemons::Sfx::IAudio> audio);
        void PlayMusic(WeakPointer<Daemons::Sfx::IMusic> music);
        
        WeakPointer<Daemons::Sfx::IAudio> GetAudio(CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileContent);
        WeakPointer<Daemons::Sfx::IMusic> GetMusic(CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileContent);

        //BaseDriver
        std::string GetDriverName() { return "SDL2"; }
        std::string GetDriverVersion() { return ESSEX_ENGINE_VERSION; }
    private:
        std::map<Daemons::Window::IRenderContext*, WeakPointer<SDL_Surface>> rgbaBuffers;
        std::map<Daemons::Window::IRenderContext*, WeakPointer<SDL_Surface>> buffers;
        std::map<Daemons::Window::IRenderContext*, WeakPointer<SDL_Renderer>> renderers;
        std::map<std::string, WeakPointer<SDL_Texture>> textureCache;
    };
}}};
