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
#include <EssexEngineSDL2Driver/SDL2Sprite.h>

EssexEngine::Drivers::SDL2::SDL2Driver::SDL2Driver(WeakPointer<Context> _context):BaseDriver(_context),
surface(
    UniquePointer<SDL_Surface>(
        SDL_CreateRGBSurface(0, 1024, 800, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff)
    )
),
renderer(
    UniquePointer<SDL_Renderer>(
        SDL_CreateSoftwareRenderer(surface)
    )
) {
    textureCache = std::map<std::string, SDL_Texture*>();
}

EssexEngine::Drivers::SDL2::SDL2Driver::~SDL2Driver() {
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

//IGfxDriver
void EssexEngine::Drivers::SDL2::SDL2Driver::Setup(WeakPointer<Daemons::Window::IRenderContext> target) {
    SDL_Init(SDL_INIT_EVERYTHING);
    
    TTF_Init();
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 255);
    

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
}

void EssexEngine::Drivers::SDL2::SDL2Driver::StartRender(WeakPointer<Daemons::Window::IRenderContext> target) {
}

void EssexEngine::Drivers::SDL2::SDL2Driver::FinishRender(WeakPointer<Daemons::Window::IRenderContext> target) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

    }
}

void EssexEngine::Drivers::SDL2::SDL2Driver::RenderEntity(WeakPointer<Daemons::Window::IRenderContext> target, WeakPointer<Daemons::Gfx::Entity> entity) {
    SDL2Sprite* sprite = (SDL2Sprite*)(entity.Get()->GetSprite().Get());
    
    glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    float texw, texh;
    SDL_GL_BindTexture(sprite->GetSprite(), &texw, &texh);
    
    GLfloat Vertices[] = {
        (float)entity.Get()->GetX(), (float)entity.Get()->GetY(), 0,
        ((float)entity.Get()->GetX() + (float)sprite->GetWidth()) * entity.Get()->GetScaleX(), (float)entity.Get()->GetY(), 0,
        ((float)entity.Get()->GetX() + (float)sprite->GetWidth()) * entity.Get()->GetScaleX(), ((float)entity.Get()->GetY() + (float)sprite->GetHeight()) * entity.Get()->GetScaleY(), 0,
        (float)entity.Get()->GetX(), ((float)entity.Get()->GetY() + (float)sprite->GetHeight()) * entity.Get()->GetScaleY(), 0};
    GLfloat TexCoord[] = {
        (float)((float)sprite->GetX() / (float)sprite->GetTotalWidth()), (float)((float)sprite->GetY() / (float)sprite->GetTotalHeight()),
        (float)(((float)sprite->GetX() + (float)sprite->GetWidth()) / (float)sprite->GetTotalWidth()), (float)((float)sprite->GetY() / (float)sprite->GetTotalHeight()),
        (float)(((float)sprite->GetX() + (float)sprite->GetWidth()) / (float)sprite->GetTotalWidth()), (float)(((float)sprite->GetY() + (float)sprite->GetHeight()) / (float)sprite->GetTotalHeight()),
        (float)((float)sprite->GetX() / (float)sprite->GetTotalWidth()), (float)(((float)sprite->GetY() + (float)sprite->GetHeight()) / (float)sprite->GetTotalHeight()),
    };
    GLubyte indices[] = {0,1,2, // first triangle (bottom left - top left - top right)
        0,2,3}; // second triangle (bottom left - top right - bottom right)
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, Vertices);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, TexCoord);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    
    SDL_GL_UnbindTexture(sprite->GetSprite());
    
}

void EssexEngine::Drivers::SDL2::SDL2Driver::RenderModel(WeakPointer<Daemons::Window::IRenderContext> target, WeakPointer<Daemons::Gfx::Model> model) {

}

void EssexEngine::Drivers::SDL2::SDL2Driver::RenderString(WeakPointer<Daemons::Window::IRenderContext> target, std::string data, int x, int y) {
}

EssexEngine::WeakPointer<EssexEngine::Daemons::Gfx::ISprite> EssexEngine::Drivers::SDL2::SDL2Driver::GetSprite(CachedPointer<std::string, Daemons::FileSystem::IFileBuffer> fileContent, int _x, int _y, int _width, int _height) {
    if (textureCache.find(fileContent->GetFileName()) == textureCache.end() ) {
        SDL_RWops* rw = SDL_RWFromMem(fileContent->GetBuffer(), fileContent->GetSize());
        SDL_Surface* surface = IMG_Load_RW(rw, 1);
        textureCache[fileContent->GetFileName()] = SDL_CreateTextureFromSurface(renderer, surface);
    }
    return EssexEngine::WeakPointer<Daemons::Gfx::ISprite>(new SDL2Sprite(std::move(fileContent), textureCache[fileContent->GetFileName()], _x, _y, _width, _height));
}
