#pragma once
#include "scene.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <vector>

class OptionsScene : public Scene {
public:
    OptionsScene(SDL_Renderer* renderer, TTF_Font* font,
                   const std::vector<std::string>& selectable_options,
                   const std::vector<std::string>& static_texts_col2 = {});
    void handle_event(SDL_Event& e) override;
    void update() override {}
    void render(SDL_Renderer* renderer) override;
    bool is_finished() const override;
    bool should_quit() const override;
    int get_selected_index() const;

protected:
    virtual void on_select(int selected_index) = 0;

    int current_selected_index;
    bool scene_finished;
    bool scene_should_quit;
    std::vector<std::string> selectable_menu_options_;
    std::vector<std::string> static_column2_texts_;
    TTF_Font* scene_font;
};
