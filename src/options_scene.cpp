#include "options_scene.h"
#include <iostream> // For potential debugging

OptionsScene::OptionsScene(SDL_Renderer* /*renderer*/, TTF_Font* font, const std::vector<std::string>& options)
    : current_selected_index(0),
      scene_finished(false),
      scene_should_quit(false),
      menu_options(options),
      scene_font(font) {}

void OptionsScene::handle_event(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        scene_should_quit = true;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                current_selected_index = (current_selected_index - 1 + menu_options.size()) % menu_options.size();
                break;
            case SDLK_DOWN:
                current_selected_index = (current_selected_index + 1) % menu_options.size();
                break;
            case SDLK_RETURN:
                on_select(current_selected_index); // Delegate action to derived class
                break;
        }
    }
}

void OptionsScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < menu_options.size(); ++i) {
        SDL_Color color = (i == static_cast<size_t>(current_selected_index)) ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255}; // Highlight yellow, else white

        SDL_Surface* surface = TTF_RenderText_Solid(scene_font, menu_options[i].c_str(), color);
        if (!surface) {
            std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
            continue; // Skip rendering this item if surface creation fails
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {100, 150 + static_cast<int>(i) * 60, surface->w, surface->h};

        SDL_FreeSurface(surface);
        if (!texture) {
             std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            continue; // Skip rendering if texture creation fails
        }
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderer);
}

bool OptionsScene::is_finished() const { return scene_finished; }
bool OptionsScene::should_quit() const { return scene_should_quit; }
int OptionsScene::get_selected_index() const { return current_selected_index; }
