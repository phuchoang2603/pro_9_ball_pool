#pragma once
#include "SDL2/SDL.h"

class Scene {
public:
    virtual ~Scene() = default;
    virtual void handle_event(SDL_Event& e) = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual bool is_finished() const = 0;
    virtual bool should_quit() const = 0;
};

