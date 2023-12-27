#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"

#define N_POINTS 9 * 9 * 9

bool is_running = false;
int previous_frame_time = 0;

vec3_t* vectors;
vec2_t* projected_points;

vec3_t camera_position = { 0.0, 0.0, -5.0 };
vec3_t cube_rotation = { .x = 0.0, .y = 0.0, .z = 0.0 };

int fov_factor = 640;

uint32_t RED = 0xFFFF0000;
uint32_t GREEN = 0xFF00FF00;
uint32_t BLUE = 0xFF0000FF;
uint32_t YELLOW = 0xFFFFFF00;
uint32_t WHITE = 0xFFFFFFFF;
uint32_t BLACK = 0xFF000000;


// NOTE:
// calling draw_pixel(200.45...) is valid because it gets casted to an int. But its not 200.45, more like 400 something

vec2_t project(vec3_t point) {
   vec2_t projected_point = {
       .x = (point.x * fov_factor) / point.z,
       .y = (point.y * fov_factor) / point.z
   };

   return projected_point;
}


void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
    }
}
// tan (alpha) = opuesto / adyacente
// 

void update(void) {
    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // total time in execution
    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = vectors[i];

        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

        transformed_point.z -= camera_position.z; 

        vec2_t projected_point = project(transformed_point);
        projected_points[i] = projected_point;
    }
}
 

void render(void) {
    draw_grid();
    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];

        draw_rect(projected_point.x + (window_width / 2), projected_point.y + (window_height / 2), 4, 4, RED);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}




void init_vectors(float width, float height, float depth, int x_density, int y_density, int z_density) {
    // width = 2
    // height = 2
    // depth = 2
    // float dx = width / 9.0; // 0.22222222
    // float dy = height / 9.0;
    // float dz = depth / 9.0;
    float dx = 0.25;
    float dy = 0.25;
    float dz = 0.25;

    float x_start = -1.0;
    float y_start = -1.0;
    float z_start = -1.0;

    for(int z = 0; z < z_density; z++){ 
        for(int y = 0; y < y_density; y++) {
            for(int x = 0; x < x_density; x++) {
                vec3_t new_point = { .x = x_start + dx * x, .y = y_start + dy * y, .z = z_start + dz * z };
                printf("(%.3f, %.3f, %.3f)", new_point.x, new_point.y, new_point.z);
                vectors[(x_density * y) + x + (z * x_density * y_density)] = new_point;
            }
        }
    }
    
}

int main(void) {
    is_running = initialize_window();
    int x_density = 9;
    int y_density = 9;
    int z_density = 9;

    vectors = malloc(sizeof(vec3_t) * x_density * y_density * z_density);
    projected_points = malloc(sizeof(vec2_t) * x_density * y_density * z_density);

    init_vectors(2, 2, 2, 9, 9, 9);


    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    free(vectors);
    free(projected_points);
    destroy_window();

    return 0;
}

/*
 *
void render(void* callback) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    callback();
    // -- calback --
    draw_grid();
    draw_rect(300, 200, 300, 150, 0xFFFF00FF);
    // -- calback --

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}
 *
 *
 *
 *
 */
