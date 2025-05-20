#include "table.h"
#include "utility.h"
#include "menu_scene.h"
#include "SDL2/SDL_ttf.h"
#include <memory>
#include <iostream>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("9 Ball Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TABLE_WIDTH, TABLE_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("assets/FSEX302.ttf", 24);

    while (true) {
        MenuScene menu(renderer, font);
        while (!menu.is_finished() && !menu.should_quit()) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                menu.handle_event(event);
            }
            menu.update();
            menu.render(renderer);
            SDL_Delay(1000 / FPS);
        }

        if (menu.should_quit()) break;

        Table table(window, renderer);
        table.run();

        if (!table.return_to_main_menu)
            break;  // quit game
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
