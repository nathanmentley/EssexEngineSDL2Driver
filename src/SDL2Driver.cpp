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

using EssexEngine::WeakPointer;

using EssexEngine::Daemons::Input::KeyboardButton::InputKeys;

using EssexEngine::Daemons::FileSystem::IFileBuffer;

using EssexEngine::Daemons::Gfx::ISprite;
using EssexEngine::Daemons::Gfx::IFont;
using EssexEngine::Daemons::Gfx::Model;
using EssexEngine::Daemons::Gfx::Entity;

using EssexEngine::Daemons::Sfx::IMusic;
using EssexEngine::Daemons::Sfx::IAudio;

using EssexEngine::Daemons::Window::IRenderContext;

using EssexEngine::Drivers::SDL2::SDL2Driver;

SDL2Driver::SDL2Driver(WeakPointer<Context> _context): 
    BaseDriver(_context) {
        buffers = std::map<Daemons::Window::IRenderContext*, WeakPointer<SDL_Surface>>();
        rgbaBuffers = std::map<Daemons::Window::IRenderContext*, WeakPointer<SDL_Surface>>();
        renderers = std::map<Daemons::Window::IRenderContext*, WeakPointer<SDL_Renderer>>();

        textureCache = std::map<std::string, WeakPointer<SDL_Texture>>();

        SetAudioListenerLocation(0, 0, 0);
    
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        Mix_Init(MIX_INIT_MP3);
        TTF_Init();
        if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0) {
            GetContext()->GetDaemon<Core::Logging::LogDaemon>()->LogLine("open audio failed");
        }
    }

SDL2Driver::~SDL2Driver() {
    Mix_CloseAudio();
    TTF_Quit();

    //Clear textureCache since they're weak pointers
    //Clear buffers
    //Clear renderers
}

//IGfxDriver
void SDL2Driver::Setup(WeakPointer<IRenderContext> target) {
    buffers[target.Get()] = SDL_CreateRGBSurface(0, target->GetWidth(), target->GetHeight(), 32, 0, 0, 0, 0);
    rgbaBuffers[target.Get()] = SDL_CreateRGBSurface(0, target->GetWidth(), target->GetHeight(), 32, 0, 0, 0, 0);
    renderers[target.Get()] = SDL_CreateSoftwareRenderer(buffers[target.Get()]);

    target->RenderToContext(WeakPointer<void>(buffers[target.Get()]->pixels));
}

void SDL2Driver::StartRender(WeakPointer<IRenderContext> target) {
    SDL_RenderClear(renderers[target.Get()]);
}

void SDL2Driver::FinishRender(WeakPointer<IRenderContext> target) {
    SDL_RenderPresent(renderers[target.Get()]);

    SDL_FreeSurface(rgbaBuffers[target.Get()]);
    rgbaBuffers[target.Get()] = SDL_ConvertSurfaceFormat(buffers[target.Get()].Get(), SDL_PIXELFORMAT_RGBA32, 0);
    target->RenderToContext(WeakPointer<void>(rgbaBuffers[target.Get()]->pixels));

    //SDL_PumpEvents();
/*
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

    }*/
}

void SDL2Driver::RenderEntity(WeakPointer<IRenderContext> target, WeakPointer<Entity> entity) {
    WeakPointer<SDL2Sprite> sprite = entity->GetSprite().Cast<SDL2Sprite>();
    
    SDL_Rect stexr;
    stexr.x = sprite->GetX();
    stexr.y = sprite->GetY();
    stexr.w = sprite->GetWidth();
    stexr.h = sprite->GetWidth();

    SDL_Rect dtexr;
    dtexr.x = entity->GetX();
    dtexr.y = entity->GetY();
    dtexr.w = sprite->GetWidth() * entity.Get()->GetScaleX();
    dtexr.h = sprite->GetWidth() * entity.Get()->GetScaleX();

    SDL_RenderCopy(renderers[target.Get()], sprite->GetSprite(), &stexr, &dtexr);
}

void SDL2Driver::RenderModel(WeakPointer<IRenderContext> target, WeakPointer<Model> model) {}

void SDL2Driver::RenderString(WeakPointer<IRenderContext> target, WeakPointer<IFont> font, std::string data, int x, int y) {
    int fontSize = font.Cast<SDL2Font>()->GetFontSize();
    SDL_Color white = {255, 255, 255};
    SDL_Rect rect;
    
    BaseUniquePointer<SDL_Surface> surface = BaseUniquePointer<SDL_Surface>(
        TTF_RenderText_Solid(
            font.Cast<SDL2Font>()->GetFont().Get(),
            data.c_str(),
            white
        ),
        [=] (SDL_Surface* ptr) { SDL_FreeSurface(ptr); } 
    );
    BaseUniquePointer<SDL_Texture> texture = BaseUniquePointer<SDL_Texture>(
        SDL_CreateTextureFromSurface(
            renderers[target.Get()],
            surface.Get()
        ),
        [=] (SDL_Texture* ptr) { SDL_DestroyTexture(ptr); } 
    );

    rect.x = x;
    rect.y = y;
    rect.w = fontSize * data.length();
    rect.h = fontSize;

    SDL_RenderCopy(renderers[target.Get()], texture, NULL, &rect);
}

WeakPointer<IFont> SDL2Driver::GetFont(WeakPointer<IRenderContext> target, CachedPointer<std::string, IFileBuffer> fileContent, int fontSize) {
    return WeakPointer<SDL2Font>(new SDL2Font(std::move(fileContent), fontSize)).Cast<IFont>();
}

WeakPointer<ISprite> SDL2Driver::GetSprite(WeakPointer<IRenderContext> target, CachedPointer<std::string, IFileBuffer> fileContent, int _x, int _y, int _width, int _height) {
    if (textureCache.find(fileContent->GetFileName()) == textureCache.end() ) {
        SDL_RWops* rw = SDL_RWFromMem(fileContent->GetBuffer(), fileContent->GetSize());
        SDL_Surface* surface = IMG_Load_RW(rw, 1);
        
        textureCache[fileContent->GetFileName()] = WeakPointer<SDL_Texture>(SDL_CreateTextureFromSurface(renderers[target.Get()], surface));
        
        SDL_FreeSurface(surface);
    }

    return WeakPointer<ISprite>(new SDL2Sprite(std::move(fileContent), textureCache[fileContent->GetFileName()], _x, _y, _width, _height));
}

//IInputDriver
bool SDL2Driver::IsKeyPressed(Daemons::Input::KeyboardButton::InputKeys key) {
    /*const Uint8* keyboardState = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));

    switch(key) {
        case InputKeys::Left:
            return keyboardState[SDL_SCANCODE_LEFT];
        case InputKeys::Right:
            return keyboardState[SDL_SCANCODE_RIGHT];
        case InputKeys::Up:
            return keyboardState[SDL_SCANCODE_UP];
        case InputKeys::Down:
            return keyboardState[SDL_SCANCODE_DOWN];
        case InputKeys::Plus:
            return keyboardState[SDL_SCANCODE_EQUALS];
        case InputKeys::Minus:
            return keyboardState[SDL_SCANCODE_MINUS];
        case InputKeys::Space:
            return keyboardState[SDL_SCANCODE_SPACE];
        case InputKeys::Esc:
            return keyboardState[SDL_SCANCODE_ESCAPE];
        case InputKeys::Tilde:
            return keyboardState[SDL_SCANCODE_GRAVE];
    }*/
    
    return false;
}

bool SDL2Driver::IsMousePressed(Daemons::Input::MouseButton::MouseButtons key, Daemons::Input::MouseEventLocation &data) {
    return false;
}

//ISfxDriver
void SDL2Driver::PlayAudio(WeakPointer<IAudio> audio) {
    int channel = Mix_PlayChannel(-1, audio.Cast<SDL2Audio>()->GetAudio(), 1);
    audio.Cast<SDL2Audio>()->SetChannel(channel);
    Mix_SetPosition(channel, 0, 0);
}

void SDL2Driver::PlayMusic(WeakPointer<IMusic> music) {
    Mix_PlayMusic(music.Cast<SDL2Music>()->GetMusic(), 1);
}

void SDL2Driver::SetAudioListenerLocation(int _x, int _y, int _z) {
    audioX = _x;
    audioY = _y;
    audioZ = _z;
}

void SDL2Driver::UpdateAudioPosition(WeakPointer<IAudio> audio, int _x, int _y, int _z) {
    int channel = audio.Cast<SDL2Audio>()->GetChannel();
    audio.Cast<SDL2Audio>()->SetPosition(_x, _y, _z);
    Mix_SetPosition(channel, 0, 0);
}

WeakPointer<IAudio> SDL2Driver::GetAudio(CachedPointer<std::string, IFileBuffer> fileContent, int _x, int _y, int _z) {
    return WeakPointer<SDL2Audio>(
        new SDL2Audio(fileContent.ToWeakPointer(), _x, _y, _z)
    ).Cast<IAudio>();
}

WeakPointer<IMusic> SDL2Driver::GetMusic(CachedPointer<std::string, IFileBuffer> fileContent) {
    return WeakPointer<SDL2Music>(new SDL2Music(fileContent.ToWeakPointer())).Cast<IMusic>();
}
