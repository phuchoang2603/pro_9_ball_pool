#ifndef TABLE_H
#define TABLE_H

#include "utility.h"
#include "ball.h"
#include "cue.h"

#include <string>
#include "SDL2/SDL_ttf.h"
#include <vector> // Should be <list> for balls if you kept it as std::list
#include <list>   // Make sure this is included for std::list<Ball> balls;

class Table {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* game_font;

    bool is_running;
    Ball cue_ball;
    Cue cue;
    std::list<Ball> balls; // Make sure <list> is included
    std::vector<Position> pockets;
    int score;

public:
    bool pause_requested = false;
    bool return_to_main_menu = false;

    Table(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

    void show_pause_menu();
    void initialize_balls();
    void reset_balls();
    void initialize_pockets();
    void process_input();
    void check_collisions();
    void check_pockets();
    void render_text(const std::string& str, int x, int y);
    void render();
    bool is_ball_in_pocket(const Position& ball_pos);
    void update();
    void run();
};

#endif
