#include "utility.h"

void render_draw_filled_circle(SDL_Renderer* renderer, int center_x, int center_y, int radius) {
    int x = 0;
    int y = radius;
    int decision = 1 - radius;

    while (x <= y) {
        SDL_RenderDrawLine(renderer, center_x - x, center_y + y, center_x + x, center_y + y);
        SDL_RenderDrawLine(renderer, center_x - x, center_y - y, center_x + x, center_y - y);
        SDL_RenderDrawLine(renderer, center_x - y, center_y + x, center_x + y, center_y + x);
        SDL_RenderDrawLine(renderer, center_x - y, center_y - x, center_x + y, center_y - x);

        ++x;
        if (decision < 0) {
            decision += 2*x + 1;
        } else {
            --y;
            decision += 2*(x-y) + 1;
        }
    }
}

bool execute_scene(Scene& scene, SDL_Renderer* renderer) {
    bool global_quit_event_occurred = false;
    while (!scene.is_finished() && !scene.should_quit() && !global_quit_event_occurred) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                global_quit_event_occurred = true; // Signal that the window close button was pressed
                break;
            }
            scene.handle_event(event);
        }
        if (global_quit_event_occurred) break;

        scene.update();
        scene.render(renderer);
        SDL_Delay(1000 / FPS); // FPS is from utility.h
    }
    return global_quit_event_occurred;
}
