#ifndef TABLE_H
#define TABLE_H

#include "scene.h"    // Base class
#include "utility.h"
#include "ball.h"
#include "cue.h"
#include "menu_scene.h" // For Difficulty enum
#include "pause_scene.h" // For creating PauseScene instance

#include <string>
#include "SDL2/SDL_ttf.h"
#include <vector>
#include <list>

class Table : public Scene {
public:
    Table(SDL_Renderer* renderer, TTF_Font* font, Difficulty difficulty);
    ~Table() override = default;

    // Scene interface overrides
    void handle_event(SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    bool is_finished() const override;
    bool should_quit() const override;

    // Specific state query for main.cpp
    bool wants_to_return_to_main_menu() const;

private:
    void initialize_balls();
    void reset_balls();
    void initialize_pockets();

    void process_game_input(SDL_Event& e); // Helper for game-specific input
    void check_collisions();
    void check_pockets();
    void render_text(SDL_Renderer* renderer, const std::string& str, int x, int y);
    bool is_ball_in_pocket(const Position& ball_pos);
    bool are_all_balls_stationary() const;

    void show_pause_menu_modal();

    SDL_Renderer* renderer_ptr_;
    TTF_Font* game_font_ptr_;
    Difficulty difficulty_level_;

    Ball cue_ball;
    Cue cue;
    std::list<Ball> balls;
    std::vector<Position> pockets;
    int score;

    // State flags
    bool return_to_main_menu_flag_;
    bool app_should_quit_flag_;
    bool shot_in_progress_;
    int balls_pocketed_this_shot_;
    bool cue_ball_was_struck_this_turn_;
};

#endif
