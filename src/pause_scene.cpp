#include "pause_scene.h"
#include <SDL2/SDL.h>

PauseScene::PauseScene(SDL_Renderer*, TTF_Font* font)
    : selected_index(0), finished(false), quit(false), return_to_menu(false), font(font) {}

void PauseScene::handle_event(SDL_Event& e) {
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
                else return_to_menu = true;
                break;
        }
    }
}

void PauseScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black screen
    SDL_RenderClear(renderer);

    for (int i = 0; i < 2; ++i) {
        SDL_Color color = (i == selected_index) ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {100, 150 + i * 60, surface->w, surface->h};
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

bool PauseScene::is_finished() const { return finished; }
bool PauseScene::should_quit() const { return quit; }
bool PauseScene::should_return_to_menu() const { return return_to_menu; }

