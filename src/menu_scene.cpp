#include "menu_scene.h"

MenuScene::MenuScene(SDL_Renderer* renderer, TTF_Font* font)
    : OptionsScene(renderer, font, {"Easy", "Hard", "Quit"}),
      selected_difficulty_(Difficulty::EASY) {}

void MenuScene::on_select(int selected_index) {
    switch (selected_index) {
            case 0: // Easy
                selected_difficulty_ = Difficulty::EASY;
                this->scene_finished = true;
                break;
            case 1: // Hard
                selected_difficulty_ = Difficulty::HARD;
                this->scene_finished = true;
                break;
            case 2: // Quit
                this->scene_should_quit = true;
                break;
        }
}

Difficulty MenuScene::get_selected_difficulty() const {
    return selected_difficulty_;
}
