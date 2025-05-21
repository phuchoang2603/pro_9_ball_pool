#define SDL_MAIN_HANDLED
#include "SDL2/SDL_ttf.h"
#include <iostream>

#include "scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "table.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("9 Ball Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TABLE_WIDTH, TABLE_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* global_font = TTF_OpenFont("assets/FSEX302.ttf", 24); 

    if (!window || !renderer || !global_font) {
        std::cerr << "Failed to initialize SDL, Renderer, or Font." << std::endl;
        if (global_font) TTF_CloseFont(global_font);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool application_running = true;
    Scene* current_scene = new MenuScene(renderer, global_font);

    while (application_running) {
        if (!current_scene) {
            application_running = false;
            std::cerr << "Error: current_scene is null!" << std::endl;
            break;
        }

        bool global_quit_event_from_execute = execute_scene(*current_scene, renderer);

        if (global_quit_event_from_execute || current_scene->should_quit()) {
            application_running = false;
        } else if (current_scene->is_finished()) {
            Scene* next_scene = nullptr;

            if (dynamic_cast<MenuScene*>(current_scene)) { // Check if it's a MenuScene instance
                MenuScene* menu = static_cast<MenuScene*>(current_scene);
                Difficulty selected_difficulty = menu->get_selected_difficulty();
                next_scene = new Table(renderer, global_font, selected_difficulty);
            } else if (dynamic_cast<Table*>(current_scene)) { // Check if it's a Table instance
                // For now, any finish from Table scene leads back to the MenuScene
                next_scene = new MenuScene(renderer, global_font);
            }

            delete current_scene;
            current_scene = next_scene;

            if (!current_scene) {
                application_running = false;
            }
        }
    }

    if (current_scene) { // Clean up the last active scene
        delete current_scene;
        current_scene = nullptr;
    }


    TTF_CloseFont(global_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
