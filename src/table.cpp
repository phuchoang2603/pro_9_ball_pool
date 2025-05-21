#include "table.h"
#include <iostream>

Table::Table(SDL_Renderer* renderer, TTF_Font* font, Difficulty difficulty)
    : renderer_ptr_(renderer),
      game_font_ptr_(font),
      difficulty_level_(difficulty),
      cue_ball({100, 200}, {255, 255, 255, 255}),
      score(0),
      return_to_main_menu_flag_(false),
      app_should_quit_flag_(false),
      shot_in_progress_(false),
      balls_pocketed_this_shot_(0),
      cue_ball_was_struck_this_turn_(false){
    initialize_balls();
    initialize_pockets();

    if (difficulty_level_ == Difficulty::EASY) {
        cue.set_guideline_active(true);
    } else { // HARD
        cue.set_guideline_active(false);
    }
}

void Table::handle_event(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            show_pause_menu_modal();
            return;
        }
    }
    process_game_input(e);
}

void Table::process_game_input(SDL_Event& e) {
    const float power_step = 1.0f;
    const float min_power = 5.0f;
    const float max_power = FPS / 3.0f;

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (!cue_ball.is_moving()) {
             int mouse_x, mouse_y;
             SDL_GetMouseState(&mouse_x, &mouse_y);
             float angle = cue.getAngle();
             cue_ball.apply_force(angle, cue.getPower());

            // Start of a new shot sequence
            this->shot_in_progress_ = true;
            this->balls_pocketed_this_shot_ = 0;
            this->cue_ball_was_struck_this_turn_ = true; // Player has taken an action
         }
    }

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            // case SDLK_g:
            //     cue.toggle_guideline();
            //     break;
            case SDLK_UP:
                cue.setPower(std::min(cue.getPower() + power_step, max_power));
                break;
            case SDLK_DOWN:
                cue.setPower(std::max(cue.getPower() - power_step, min_power));
                break;
        }
    }
}

void Table::update() {
    cue_ball.move();
    for (Ball& ball : balls) {
        ball.move();
    }

    if (is_ball_in_pocket(cue_ball.get_position())) {
        score -= 5;
        cue_ball.reset();
    }

    if (shot_in_progress_ && are_all_balls_stationary()) {
        shot_in_progress_ = false;

        // Apply penalty if a shot was made and no object balls were pocketed
        if (cue_ball_was_struck_this_turn_ && balls_pocketed_this_shot_ == 0) {
            score--;
        }
        
        cue_ball_was_struck_this_turn_ = false; // Reset for the next turn/shot attempt
    }

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    cue.update(cue_ball.get_position(), mouse_x, mouse_y);

    check_collisions();
    check_pockets();

    if (balls.empty()) {
        reset_balls();
    }
}

void Table::render(SDL_Renderer* renderer_param) {
    SDL_SetRenderDrawColor(renderer_param, 0, 100, 0, 255);
    SDL_RenderClear(renderer_param);

    SDL_SetRenderDrawColor(renderer_param, 0, 0, 0, 255);
    for (const Position& pocket : pockets) {
        render_draw_filled_circle(renderer_param, pocket.x, pocket.y, POCKET_RADIUS);
    }

    cue_ball.draw(renderer_param);
    for (const Ball& ball : balls) {
        ball.draw(renderer_param);
    }

    cue.draw(renderer_param, cue_ball.get_position());
    cue.draw_guideline(renderer_param, cue_ball.get_position(), BALL_RADIUS, TABLE_WIDTH, TABLE_HEIGHT, balls);

    render_text(renderer_param, "Score: " + std::to_string(score), 40, 20);
    render_text(renderer_param, "Power: " + std::to_string((int)cue.getPower()), TABLE_WIDTH - 150, 20);

    SDL_RenderPresent(renderer_param);
}

void Table::render_text(SDL_Renderer* renderer_param, const std::string& str, int x, int y) {
    SDL_Color text_color = {255, 255, 255, 255}; // white

    SDL_Surface* text_surface = TTF_RenderText_Solid(game_font_ptr_, str.c_str(), text_color);
    if (!text_surface) {
        std::cerr << "Text Rendering Error: " << TTF_GetError() << "\n";
        return;
    }

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer_param, text_surface);
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};

    SDL_FreeSurface(text_surface);
    if (!text_texture) {
        std::cerr << "Texture Creation Error: " << SDL_GetError() << "\n";
        return;
    }

    SDL_RenderCopy(renderer_param, text_texture, nullptr, &text_rect);
    SDL_DestroyTexture(text_texture);
}

bool Table::is_finished() const {
    // Game table scene finishes if it wants to return to menu,
    // or if a game over condition that leads to another scene is met.
    return return_to_main_menu_flag_;
}

bool Table::should_quit() const {
    // Game table scene signals a full app quit if necessary.
    return app_should_quit_flag_;
}

bool Table::wants_to_return_to_main_menu() const {
    return return_to_main_menu_flag_;
}

void Table::show_pause_menu_modal() {
    PauseScene pause(renderer_ptr_, game_font_ptr_);
    
    bool global_quit_in_pause = execute_scene(pause, renderer_ptr_);

    if (global_quit_in_pause || pause.should_quit()) {
        this->app_should_quit_flag_ = true;
    } else if (pause.should_return_to_menu()) {
        this->return_to_main_menu_flag_ = true;
    }
}

void Table::initialize_balls() {
    balls.clear();
    balls.emplace_back(Position{400, 200}, SDL_Color{255, 255, 0, 255});
    balls.emplace_back(Position{420, 190}, SDL_Color{0, 0, 255, 255});
    balls.emplace_back(Position{420, 210}, SDL_Color{255, 0, 0, 255});
    balls.emplace_back(Position{440, 180}, SDL_Color{255, 165, 0, 255});
    balls.emplace_back(Position{440, 200}, SDL_Color{0, 128, 0, 255});
    balls.emplace_back(Position{440, 220}, SDL_Color{128, 0, 128, 255});
    balls.emplace_back(Position{460, 210}, SDL_Color{255, 20, 147, 255});
    balls.emplace_back(Position{460, 190}, SDL_Color{0, 128, 128, 255});
    balls.emplace_back(Position{480, 200}, SDL_Color{128, 0, 0, 255});
}

void Table::reset_balls() {
    initialize_balls();
    cue_ball.reset();
}

void Table::initialize_pockets() {
    const int offset = 20;

    pockets = {
        {offset, offset},
        {TABLE_WIDTH - offset, offset},
        {offset, TABLE_HEIGHT - offset},
        {TABLE_WIDTH - offset, TABLE_HEIGHT - offset},
        {(int)(TABLE_WIDTH/2), offset},
        {(int)(TABLE_WIDTH/2), TABLE_HEIGHT - offset}
    };
}

void Table::check_collisions() {
    // check collision between cueball and other balls
    for (Ball& ball : balls) {
        if (cue_ball.check_collision(ball)) {
            cue_ball.resolve_collision(ball);
        }
    }

    // now non cueballs against other non cueballs
    std::list<Ball>::iterator i,j;
    for (i=balls.begin(); i!=balls.end(); ++i){
        j=i;
        ++j; // one ahead of iterator i
        for (;j!=balls.end(); ++j){
            if (i->check_collision(*j)){
                i->resolve_collision(*j); // handles the movement of both balls
            }
        }
    }
}

void Table::check_pockets() {
    std::list<Ball>::iterator i = balls.begin();
    while (i!=balls.end()){
        if (this->is_ball_in_pocket(i->get_position())) {
            i = balls.erase(i);
            ++score;
            if (shot_in_progress_) {
                balls_pocketed_this_shot_++;
            }
        }else{
            ++i;
        }
    }
}

bool Table::are_all_balls_stationary() const {
    if (cue_ball.is_moving()) {
        return false;
    }
    for (const Ball& ball : balls) {
        if (ball.is_moving()) {
            return false;
        }
    }
    return true;
}

bool Table::is_ball_in_pocket(const Position& ball_pos) {
    float dx, dy, dist;
    for (const Position& pocket : pockets) {
        dx = ball_pos.x - pocket.x;
        dy = ball_pos.y - pocket.y;
        dist = std::sqrt(dx*dx + dy*dy);
        if (dist <= POCKET_RADIUS) {
            return true;
        }
    }
    return false;
}
