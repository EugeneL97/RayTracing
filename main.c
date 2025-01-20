#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE 0xffffff
#define COLOR_YELLOW 0xffff00
#define COLOR_BLACK 0x000000
#define RAYS_NUMBER 100

struct Circle {
    double x;
    double y;
    double radius;
};

struct Ray
{
    double x_start, y_start;
    double angle;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
    double radius_squared = pow(circle.radius, 2);
    for (double x = circle.x - circle.radius; x <= circle.x + circle.radius; ++x)
    {
        for (double y = circle.y - circle.radius; y <= circle.y + circle.radius; ++y)
        {
            double distance_squared = pow(x-circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared < radius_squared)
            {
                SDL_Rect pixel = (SDL_Rect){x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void GenerateRays(struct Circle circle, struct Ray rays[RAYS_NUMBER])
{
    for (int i = 0; i < RAYS_NUMBER; ++i)
    {
        double angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
        struct Ray ray = {circle.x, circle.y, angle};

        rays[i] = ray;
    }
}

void FillRays(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER], Uint32 color)
{
    for (int i = 0; i < RAYS_NUMBER; ++i)
    {
        struct Ray ray = rays[i];

        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1;
        double x_draw = ray.x_start;
        double y_draw = ray.y_start;
        while (!end_of_screen && !object_hit)
        {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            SDL_Rect pixel = (SDL_Rect){x_draw,y_draw,1,1};
            SDL_FillRect(surface, &pixel, color);

            if (x_draw < 0 || x_draw > WIDTH || y_draw < 0 || y_draw > HEIGHT)
            {
                end_of_screen = 1;
            }
        }
    }
}
int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle circle = {200, 200, 80};
    struct Circle circle_shadow = {650, 300, 140};

    struct Ray rays[RAYS_NUMBER];
    GenerateRays(circle, rays);

    int simulation_running = 1;
    SDL_Event event;
    while (simulation_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                simulation_running = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                GenerateRays(circle, rays);
            }
        }
        SDL_FillRect(surface, NULL, COLOR_BLACK);
        FillCircle(surface, circle, COLOR_WHITE);

        FillCircle(surface, circle_shadow, COLOR_WHITE);
        FillRays(surface, rays, COLOR_YELLOW);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(1000/144);
    }
}