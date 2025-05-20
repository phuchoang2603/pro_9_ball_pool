#include "menu_scene.h"
#include <iostream>

MenuScene::MenuScene(SDL_Renderer*, TTF_Font* font)
    : selected_index(0), finished(false), quit(false), font(font) {}

void MenuScene::handle_event(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        quit = true;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                selected_index = (selected_index - 1 + 2) % 2;
                break;
            case SDLK_DOWN:
                selected_index = (selected_index + 1) % 2;
                break;
            case SDLK_RETURN:
                if (selected_index == 0) finished = true;
                else quit = true;
                break;
        }
    }
}

void MenuScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 2; ++i) {
        SDL_Color color = {255, 255, 255, 255};
        if (i == selected_index) color = {255, 255, 0, 255}; // Highlight

        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {100, 150 + i * 60, surface->w, surface->h};

        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

bool MenuScene::is_finished() const { return finished; }
bool MenuScene::should_quit() const { return quit; }

