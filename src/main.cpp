#define SDL_MAIN_HANDLED
#include "SDL2/SDL_ttf.h"
#include <iostream>

#include "scene.h"       // Base class for scenes
#include "utility.h"     // For execute_scene, constants like TABLE_WIDTH, TABLE_HEIGHT, FPS
#include "menu_scene.h"  // For MenuScene and Difficulty enum
#include "table.h"       // For Table scene

// Global-like static variables to store scores across scenes (session scores)
static int highScore = 0;
static int lastScore = 0;
static bool hasPlayedAtLeastOnce = false; // To know if lastScore is meaningful

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("9 Ball Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TABLE_WIDTH, TABLE_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* global_font = TTF_OpenFont("assets/FSEX302.ttf", 24);
    if (!global_font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool application_running = true;
    // Initial MenuScene creation, passing initial score states
    Scene* current_scene = new MenuScene(renderer, global_font, lastScore, highScore, hasPlayedAtLeastOnce);

    while (application_running) {
        if (!current_scene) { // Safety check, should ideally not be hit
            std::cerr << "Error: current_scene is null! Exiting." << std::endl;
            application_running = false;
            break;
        }

        bool global_quit_event_from_execute = execute_scene(*current_scene, renderer); // From utility.h/cpp

        if (global_quit_event_from_execute || current_scene->should_quit()) {
            application_running = false;
        } else if (current_scene->is_finished()) {
            Scene* next_scene = nullptr;

            if (MenuScene* menu_ptr = dynamic_cast<MenuScene*>(current_scene)) {
                Difficulty selected_difficulty = menu_ptr->get_selected_difficulty();
                next_scene = new Table(renderer, global_font, selected_difficulty);
            } else if (Table* table_ptr = dynamic_cast<Table*>(current_scene)) {
                if (table_ptr->is_finished()) { // Check if the table scene itself is finished
                    lastScore = table_ptr->get_score();
                    hasPlayedAtLeastOnce = true;
                    if (lastScore > highScore) {
                        highScore = lastScore;
                    }
                    // wants_to_return_to_main_menu() will be true here if balls.empty() triggered it
                    next_scene = new MenuScene(renderer, global_font, lastScore, highScore, hasPlayedAtLeastOnce);
                }
            }

            delete current_scene;
            current_scene = next_scene;

            if (!current_scene) {
                std::cerr << "Error: next_scene is null after transition! Exiting." << std::endl;
                application_running = false;
            }
        }
    }

    if (current_scene) {
        delete current_scene;
        current_scene = nullptr;
    }

    // Cleanup SDL resources
    TTF_CloseFont(global_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
