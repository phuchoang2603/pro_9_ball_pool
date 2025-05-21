#pragma once
#include "options_scene.h" // Changed from scene.h

enum class Difficulty {
    EASY,
    HARD
};

class MenuScene : public OptionsScene {
public:
    MenuScene(SDL_Renderer* renderer, TTF_Font* font, int last_score, int high_score, bool display_score_info);
    Difficulty get_selected_difficulty() const;

private:
    void on_select(int selected_index) override;
    Difficulty selected_difficulty_;
    int last_score_value_;
    int high_score_value_;
    bool display_score_info_; // To know if scores should be displayed (e.g., after first game)
};
