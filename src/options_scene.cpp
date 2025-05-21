#include "options_scene.h"
#include <iostream>
#include "utility.h"

OptionsScene::OptionsScene(SDL_Renderer* /*renderer*/, TTF_Font* font, 
                           const std::vector<std::string>& selectable_options,
                           const std::vector<std::string>& static_texts_col2)
    : current_selected_index(0),
      scene_finished(false),
      scene_should_quit(false),
      selectable_menu_options_(selectable_options),
      static_column2_texts_(static_texts_col2),
      scene_font(font) {}

void OptionsScene::handle_event(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        scene_should_quit = true;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                current_selected_index = (current_selected_index - 1 + selectable_menu_options_.size()) % selectable_menu_options_.size();
                break;
            case SDLK_DOWN:
                current_selected_index = (current_selected_index + 1) % selectable_menu_options_.size();
                break;
            case SDLK_RETURN:
                on_select(current_selected_index); 
                break;
        }
    }
}

void OptionsScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Column 1: Selectable Options
    int col1_x = 100;
    int col1_y_start = 150;
    int col1_y_spacing = 60;

    for (size_t i = 0; i < selectable_menu_options_.size(); ++i) {
        SDL_Color color = (i == static_cast<size_t>(current_selected_index)) ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255}; // Highlight yellow, else white

        SDL_Surface* surface = TTF_RenderText_Solid(scene_font, selectable_menu_options_[i].c_str(), color);
        if (!surface) {
            std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
            continue; // Skip rendering this item if surface creation fails
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {col1_x, col1_y_start + static_cast<int>(i) * col1_y_spacing, surface->w, surface->h};

        SDL_FreeSurface(surface);
        if (!texture) {
             std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            continue; // Skip rendering if texture creation fails
        }
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }

    // Column 2: Static Texts
    int col2_x = TABLE_WIDTH / 2 + 30; // Position it to the right
    int col2_y_start = 150;
    int col2_y_spacing = 60; // Potentially different spacing for static info
    SDL_Color static_text_color = {200, 200, 200, 255}; // Light grey for static text

    for (size_t i = 0; i < static_column2_texts_.size(); ++i) {
        if (static_column2_texts_[i].empty()) continue; // Skip empty strings

        SDL_Surface* surface = TTF_RenderText_Solid(scene_font, static_column2_texts_[i].c_str(), static_text_color);
        if (!surface) {
            std::cerr << "TTF_RenderText_Solid Error (Col2): " << TTF_GetError() << std::endl;
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {col2_x, col2_y_start + static_cast<int>(i) * col2_y_spacing, surface->w, surface->h};
        
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "SDL_CreateTextureFromSurface Error (Col2): " << SDL_GetError() << std::endl;
            continue;
        }
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer); // Present once after all drawing
}

bool OptionsScene::is_finished() const { return scene_finished; }
bool OptionsScene::should_quit() const { return scene_should_quit; }
int OptionsScene::get_selected_index() const { return current_selected_index; }
