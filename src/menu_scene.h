#pragma once
#include "options_scene.h" // Changed from scene.h

enum class Difficulty {
    EASY,
    HARD
};

class MenuScene : public OptionsScene {
public:
    MenuScene(SDL_Renderer* renderer, TTF_Font* font);
    Difficulty get_selected_difficulty() const;

private:
    void on_select(int selected_index) override;
    Difficulty selected_difficulty_;
};
