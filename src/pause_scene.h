#pragma once
#include "scene.h"
#include "SDL2/SDL_ttf.h"
#include <string>

class PauseScene : public Scene {
public:
    PauseScene(SDL_Renderer* renderer, TTF_Font* font);
    void handle_event(SDL_Event& e) override;
    void update() override {}
    void render(SDL_Renderer* renderer) override;
    bool is_finished() const override;
    bool should_quit() const override;
    bool should_return_to_menu() const;

private:
    int selected_index;
    bool finished;
    bool quit;
    bool return_to_menu;
    std::string options[2] = {"Resume", "Main Menu"};
    TTF_Font* font;
};

