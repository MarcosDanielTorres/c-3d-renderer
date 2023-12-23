#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window* window = NULL;
uint32_t window_width = 800;
uint32_t window_height = 600;

SDL_Renderer* renderer = NULL;

SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;


bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    /*
     * SDL_DisplayMode display_mode;
     * SDL_GetCurrentDisplayMode(0, &display_mode);
     * window_width = display_mode.w;
     * window_height = display_mode.h;
     *
     * window_width = 800;
     * window_height = 600;
     */


    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void setup(void){
    is_running = initialize_window();

    color_buffer = malloc(window_width * window_height * sizeof(uint32_t));

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
}

void process_input(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
            }
            break;
    }
}

void update(void){

}

void clear_color_buffer(uint32_t color) {
    for (int i = 0; i < window_height; i++) {
        for (int j = 0; j < window_width; j++) {
            color_buffer[(window_width * i) + j] = color;
        }
    }
}

void render_color_buffer() {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int) (window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_grid(uint32_t* color_buffer) {
    uint32_t grid_color = 0xFF0000FF;
    for (int i = 0; i < window_height; i++) {
        for (int j = 0; j < window_width; j++) {
            if (i % 10 == 0 || j % 10 == 0) {
                color_buffer[(window_width * i) + j] = grid_color;
            }
        }
    }
}

void render(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    clear_color_buffer(0xFFFFFF00);
    draw_grid(color_buffer);
    render_color_buffer();

    SDL_RenderPresent(renderer);
}

void destroy(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    setup();


    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy();
    return 0;
}
