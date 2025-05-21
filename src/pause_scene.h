#pragma once
#include "options_scene.h" // Changed from scene.h

class PauseScene : public OptionsScene {
public:
    PauseScene(SDL_Renderer* renderer, TTF_Font* font);
    bool should_return_to_menu() const; // Specific to PauseScene

private:
    void on_select(int selected_index) override;
    bool wants_to_return_to_menu;
};
