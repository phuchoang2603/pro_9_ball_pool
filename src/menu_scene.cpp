#include "menu_scene.h"

MenuScene::MenuScene(SDL_Renderer* renderer, TTF_Font* font)
    : OptionsScene(renderer, font, {"Start Game", "Quit"}) {} // Pass options to base

void MenuScene::on_select(int selected_index) {
    if (selected_index == 0) { // "Start Game"
        scene_finished = true;
    } else if (selected_index == 1) { // "Quit"
        scene_should_quit = true;
    }
}
