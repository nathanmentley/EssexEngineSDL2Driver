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

ImVec4 clear_color = ImColor(0, 0, 0);

EssexEngine::Drivers::SDL2::SDL2Driver::SDL2Driver(WeakPointer<Context> _context):BaseDriver(_context) {
    textureCache = std::map<std::string, SDL_Texture*>();
    StartTimer();
}

EssexEngine::Drivers::SDL2::SDL2Driver::~SDL2Driver() {
    ImGui_ImplSdl_Shutdown();
    
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//IGfxDriver
void EssexEngine::Drivers::SDL2::SDL2Driver::Setup() {
    SDL_Init(SDL_INIT_EVERYTHING);
    
    TTF_Init();
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 255);
    
    ImGui_ImplSdl_Init(window);
    
    
    window = SDL_CreateWindow(
        "Game Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        GetScreenWidth(),
        GetScreenHeight(),
        SDL_WINDOW_OPENGL
    );
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
}

int EssexEngine::Drivers::SDL2::SDL2Driver::GetScreenWidth() {
    return 1028;
}

int EssexEngine::Drivers::SDL2::SDL2Driver::GetScreenHeight() {
    return 768;
}

void EssexEngine::Drivers::SDL2::SDL2Driver::StartRender() {
    ImGui_ImplSdl_NewFrame(window);
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void EssexEngine::Drivers::SDL2::SDL2Driver::FinishRender() {
    ImGui::Render();
    SDL_GL_SwapWindow(window);
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSdl_ProcessEvent(&event);
    }
}

void EssexEngine::Drivers::SDL2::SDL2Driver::RenderEntity(WeakPointer<Daemons::Gfx::Entity> entity) {
    SDL2Sprite* sprite = (SDL2Sprite*)(entity.Get()->GetSprite().Get());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
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

void EssexEngine::Drivers::SDL2::SDL2Driver::RenderModel(WeakPointer<Daemons::Gfx::Model> model) {

}

void EssexEngine::Drivers::SDL2::SDL2Driver::RenderString(std::string data, int x, int y) {
    /*
    TTF_Font* font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 24);
    
    SDL_Color text_color = {255, 255, 255};
    SDL_Surface* text = TTF_RenderText_Solid(font, data.c_str(), text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text);
    
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dstrect = { x, y, x + w, y + h };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(text);
    
    TTF_CloseFont(font);
    */
}

EssexEngine::WeakPointer<EssexEngine::Daemons::Gfx::ISprite> EssexEngine::Drivers::SDL2::SDL2Driver::GetSprite(CachedPointer<Daemons::FileSystem::IFileBuffer> fileContent, int _x, int _y, int _width, int _height) {
    if (textureCache.find(fileContent->GetFileName()) == textureCache.end() ) {
        SDL_RWops* rw = SDL_RWFromMem(fileContent->GetBuffer(), fileContent->GetSize());
        SDL_Surface* surface = IMG_Load_RW(rw, 1);
        textureCache[fileContent->GetFileName()] = SDL_CreateTextureFromSurface(renderer, surface);
    }
    return EssexEngine::WeakPointer<Daemons::Gfx::ISprite>(new SDL2Sprite(std::move(fileContent), textureCache[fileContent->GetFileName()], _x, _y, _width, _height));
}

//IInputDriver
bool EssexEngine::Drivers::SDL2::SDL2Driver::IsKeyPressed(Daemons::Input::KeyboardButton::InputKeys key) {
    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    switch(key) {
        case Daemons::Input::KeyboardButton::Left:
            return state[SDL_SCANCODE_LEFT];
        case Daemons::Input::KeyboardButton::Right:
            return state[SDL_SCANCODE_RIGHT];
        case Daemons::Input::KeyboardButton::Up:
            return state[SDL_SCANCODE_UP];
        case Daemons::Input::KeyboardButton::Down:
            return state[SDL_SCANCODE_DOWN];
        case Daemons::Input::KeyboardButton::Plus:
            return state[SDL_SCANCODE_EQUALS];
        case Daemons::Input::KeyboardButton::Minus:
            return state[SDL_SCANCODE_MINUS];
        case Daemons::Input::KeyboardButton::Space:
            return state[SDL_SCANCODE_SPACE];
        case Daemons::Input::KeyboardButton::Esc:
            return state[SDL_SCANCODE_ESCAPE];
        case Daemons::Input::KeyboardButton::Tilde:
            return state[SDL_SCANCODE_GRAVE];
    }
    
    return false;
}

bool EssexEngine::Drivers::SDL2::SDL2Driver::IsMousePressed(Daemons::Input::MouseButton::MouseButtons key, Daemons::Input::MouseEventLocation &data) {
    
    switch(key) {
        case Daemons::Input::MouseButton::MouseLeft:
            return SDL_GetMouseState(&data.x, &data.y) & SDL_BUTTON(SDL_BUTTON_LEFT);
        case Daemons::Input::MouseButton::MouseRight:
            return SDL_GetMouseState(&data.x, &data.y) & SDL_BUTTON(SDL_BUTTON_RIGHT);
        case Daemons::Input::MouseButton::MouseMiddle:
            return SDL_GetMouseState(&data.x, &data.y) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    }
}


//ISystemDriver
void EssexEngine::Drivers::SDL2::SDL2Driver::StartTimer() {
    lastTicks = SDL_GetTicks();
}

int EssexEngine::Drivers::SDL2::SDL2Driver::GetElapsedTime() {
    Uint32 ticks = SDL_GetTicks();
    return ticks - lastTicks;
}

void EssexEngine::Drivers::SDL2::SDL2Driver::Sleep(int milliseconds) {
    SDL_Delay(milliseconds);
}


