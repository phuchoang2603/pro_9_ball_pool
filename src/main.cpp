#define SDL_MAIN_HANDLED
#include "SDL2/SDL_ttf.h"
#include <memory>
#include <iostream>

#include "utility.h"
#include "menu_scene.h"
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

    while (application_running) {
        MenuScene menu(renderer, global_font); 
        bool global_quit_in_menu = execute_scene(menu, renderer);

        if (global_quit_in_menu || menu.should_quit()) {
            application_running = false;
            break;
        }

        if (menu.is_finished()) {
            Table table(window, renderer, global_font);
            table.run();

            if (!table.return_to_main_menu) {
                application_running = false;
            }
        } else {
            application_running = false;
        }
    }


    TTF_CloseFont(global_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
