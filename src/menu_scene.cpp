#include "menu_scene.h"
#include <string>

MenuScene::MenuScene(SDL_Renderer* renderer, TTF_Font* font, int last_score, int high_score, bool display_score_info)
    : OptionsScene(renderer, font, 
                   {"Easy", "Hard", "Quit"}, // Selectable options for column 1
                   // Prepare static texts for column 2
                   [&]() { // Use a lambda to prepare the vector
                       std::vector<std::string> col2_texts;
                       if (display_score_info) {
                           col2_texts.push_back("Last Score: " + std::to_string(last_score));
                       } else {
                           col2_texts.push_back("Last Score: N/A");
                       }
                       col2_texts.push_back("High Score: " + std::to_string(high_score));
                       return col2_texts;
                   }()),
      selected_difficulty_(Difficulty::EASY), // Default difficulty for game start
      last_score_value_(last_score),          // Still store for potential direct access if needed
      high_score_value_(high_score),        // though OptionsScene now handles display
      display_score_info_(display_score_info) { 
}

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
