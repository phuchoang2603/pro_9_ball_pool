#pragma once
#include "scene.h"
#include "SDL2/SDL_ttf.h"
#include <string>

class MenuScene : public Scene {
public:
    MenuScene(SDL_Renderer* renderer, TTF_Font* font);
    void handle_event(SDL_Event& e) override;
    void update() override {}
    void render(SDL_Renderer* renderer) override;
    bool is_finished() const override;
    bool should_quit() const override;

private:
    int selected_index;
    bool finished;
    bool quit;
    std::string options[2] = {"Start Game", "Quit"};
    TTF_Font* font;
};

