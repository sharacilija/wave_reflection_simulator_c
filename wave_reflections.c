#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define WIDTH 900
#define HEIGHT 600

#define WAVE_SPEED 150
#define PARTICLE_SIZE 4
#define PARTICLES_PER_WAVE 10000
#define TOTAL_POSSIBLE_PARTICLES PARTICLES_PER_WAVE*10

struct Particle {
    float x, y, vx, vy;
};

int emitted_particles = 0;
int slot_index = 0;
struct Particle particles[TOTAL_POSSIBLE_PARTICLES];

void emit_wave(Vector2 origin)
{
    // emit thousands of individual particles in different circular directions
    if (slot_index + PARTICLES_PER_WAVE >= TOTAL_POSSIBLE_PARTICLES)
    {
        slot_index = 0;
    }
    int start_index = slot_index;
    int end_index = start_index + PARTICLES_PER_WAVE;
    printf("start_index = %d\n", start_index);
    printf("end_index = %d\n", end_index);
    for (int i = start_index; i < end_index; i++)
    {
        particles[i].x = origin.x;
        particles[i].y = origin.y;
        float angle = ((float)(i - start_index) / PARTICLES_PER_WAVE) * 2 * PI;
        particles[i].vx = cosf(angle) * WAVE_SPEED;
        particles[i].vy = sinf(angle) * WAVE_SPEED;
    }
    emitted_particles += PARTICLES_PER_WAVE;
    slot_index += PARTICLES_PER_WAVE;
    printf("emitted_particles = %d\n", emitted_particles);
}

void move_wave(float dt)
{
    for (int i = 0; i < MIN(emitted_particles, TOTAL_POSSIBLE_PARTICLES); i++)
    {
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;

        if (particles[i].x < 0)
        {
            particles[i].x = 0;
            particles[i].vx = -particles[i].vx;
        }
            if (particles[i].y < 0)
        {
            particles[i].y = 0;
            particles[i].vy = -particles[i].vy;
        }
        if (particles[i].x + PARTICLE_SIZE > WIDTH)
        {
            particles[i].x = WIDTH - PARTICLE_SIZE;
            particles[i].vx = -particles[i].vx;
        }
        /*    if (particles[i].y + PARTICLE_SIZE > HEIGHT)
        {
            particles[i].y = HEIGHT - PARTICLE_SIZE;
            particles[i].vy = -particles[i].vy;
        }*/
    }
}

void draw_wave()
{
    for (int i = 0; i < MIN(emitted_particles, TOTAL_POSSIBLE_PARTICLES); i++)
    {
        DrawRectangle(particles[i].x, particles[i].y, PARTICLE_SIZE, PARTICLE_SIZE, WHITE);
    }
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Waves Reflection Simulation");

    SetTargetFPS(60);

    float interval = 0;
    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();
        interval += dt;
        Vector2 mouse_pos = GetMousePosition();
        if (interval >= 1)
        {
            emit_wave(mouse_pos);
            interval = 0;
        }

        move_wave(dt);
        BeginDrawing();
        ClearBackground(BLACK);

        draw_wave();

        EndDrawing();
    }
    return 0;
}

/*
    for further updates:
    git add .
    git commit -m "describe updates"
    git push
*/