#pragma once
#include "options_scene.h" // Changed from scene.h

class MenuScene : public OptionsScene {
public:
    MenuScene(SDL_Renderer* renderer, TTF_Font* font);

private:
    void on_select(int selected_index) override;
};
