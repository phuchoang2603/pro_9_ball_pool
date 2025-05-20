#include "table.h"
#include "menu_scene.h"
#include "SDL2/SDL_ttf.h"
#include <memory>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1) {
        std::cerr << "Initialization Error\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("9 Ball Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TABLE_WIDTH, TABLE_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("assets/FSEX302.ttf", 24);

    if (!window || !renderer || !font) {
        std::cerr << "Resource Initialization Failed\n";
        return 1;
    }

    std::unique_ptr<Scene> scene = std::make_unique<MenuScene>(renderer, font);

    while (!scene->is_finished() && !scene->should_quit()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            scene->handle_event(event);
        }
        scene->update();
        scene->render(renderer);
        SDL_Delay(1000 / FPS);
    }

    if (!scene->should_quit()) {
        Table table(window, renderer);
        table.run();
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

