#include "pause_scene.h"

PauseScene::PauseScene(SDL_Renderer* renderer, TTF_Font* font)
    : OptionsScene(renderer, font, {"Resume", "Main Menu"}), // Pass options to base
      wants_to_return_to_menu(false) {}

void PauseScene::on_select(int selected_index) {
    if (selected_index == 0) { // "Resume"
        scene_finished = true;
    } else if (selected_index == 1) { // "Main Menu"
        wants_to_return_to_menu = true;
        // scene_finished = true; // Or set finished to exit pause loop
    }
}

bool PauseScene::should_return_to_menu() const { return wants_to_return_to_menu; }
