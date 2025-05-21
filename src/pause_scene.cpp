#include "pause_scene.h"

PauseScene::PauseScene(SDL_Renderer* renderer, TTF_Font* font)
    : OptionsScene(renderer, font, 
                   {"Resume", "Main Menu"}, // Selectable options
                   {}),            
      wants_to_return_to_menu(false) {}

void PauseScene::on_select(int selected_index) {
    if (selected_index == 0) {
        this->scene_finished = true;
        this->wants_to_return_to_menu = false;
    } else if (selected_index == 1) {
        this->wants_to_return_to_menu = true;
        this->scene_finished = true;
    }
}

bool PauseScene::should_return_to_menu() const { return wants_to_return_to_menu; }
