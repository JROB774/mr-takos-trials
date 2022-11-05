#define NK_STATIC

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <nk_define.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>

#include "game.h"

#include "game.c"

#define WINDOW_TITLE  "Tako Typing Teacher"
#define WINDOW_XPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_YPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_FLAGS  SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL

static SDL_Window*   g_window;
static SDL_GLContext g_glcontext;

static void fatal_error(const nkChar* fmt, ...)
{
    nkChar message_buffer[1024] = NK_ZERO_MEM;

    va_list args;

    va_start(args, fmt);
    vsnprintf(message_buffer, NK_ARRAY_SIZE(message_buffer), fmt, args);
    va_end(args);

    #if defined(BUILD_DEBUG)
    fprintf(stderr, "%s\n", message_buffer);
    #endif

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", message_buffer, g_window);

    abort();
}

static nkBool get_fullscreen(void)
{
    return NK_CHECK_FLAGS(SDL_GetWindowFlags(g_window), SDL_WINDOW_FULLSCREEN_DESKTOP);
}

static void set_fullscreen(nkBool enable)
{
    SDL_SetWindowFullscreen(g_window, (enable) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    SDL_ShowCursor(get_fullscreen() ? SDL_DISABLE : SDL_ENABLE); // Hide the cursor in fullscreen mode.
}

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fatal_error("Failed to initialize SDL systems: %s", SDL_GetError());
    }

    g_window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_XPOS,WINDOW_YPOS, WINDOW_WIDTH,WINDOW_HEIGHT, WINDOW_FLAGS);
    if(!g_window)
    {
        fatal_error("Failed to create application window: %s", SDL_GetError());
    }

    g_glcontext = SDL_GL_CreateContext(g_window);
    if(!g_glcontext)
    {
        fatal_error("Failed to create OpenGL context: %s", SDL_GetError());
    }

    // Enable VSync by default, if we don't get it then oh well.
    if(SDL_GL_SetSwapInterval(1) == 0)
    {
        printf("VSync Enabled!\n");
    }

    game_init();

    nkU64 perf_frequency = SDL_GetPerformanceFrequency();
    nkU64 last_counter = SDL_GetPerformanceCounter();
    nkU64 end_counter = 0;
    nkU64 elapsed_counter = 0;
    nkF32 update_timer = 0.0f;

    nkF32 dt = 1.0f / 60.0f; // We use a fixed update rate to keep things deterministic.

    nkBool running = NK_TRUE;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case(SDL_KEYDOWN):
                {
                    switch(event.key.keysym.sym)
                    {
                        case(SDLK_RETURN): if(!(SDL_GetModState()&KMOD_ALT)) break;
                        case(SDLK_F11):
                        {
                            set_fullscreen(!get_fullscreen());
                        } break;
                    }
                } break;
                case(SDL_QUIT):
                {
                    running = NK_FALSE;
                } break;
            }
        }

        nkBool did_update = NK_FALSE;
        while(update_timer >= dt)
        {
            did_update = NK_TRUE;
            game_update(dt);
            update_timer -= dt;
        }
        if(did_update)
        {
            glClearColor(0.0f,0.0f,0.0f,1.0f); // @Incomplete: Do we want these here?
            glClear(GL_COLOR_BUFFER_BIT);

            game_render();
        }

        SDL_GL_SwapWindow(g_window);

        end_counter = SDL_GetPerformanceCounter();
        elapsed_counter = end_counter - last_counter;
        last_counter = SDL_GetPerformanceCounter();

        nkF32 elapsed_time = NK_CAST(nkF32,elapsed_counter) / NK_CAST(nkF32,perf_frequency);

        update_timer += elapsed_time;

        #if defined(BUILD_DEBUG)
        nkF32 current_fps = NK_CAST(nkF32,perf_frequency) / NK_CAST(nkF32,elapsed_counter);
        nkChar title_buffer[1024] = NK_ZERO_MEM;
        snprintf(title_buffer, NK_ARRAY_SIZE(title_buffer), "%s (FPS: %f)", WINDOW_TITLE, current_fps);
        SDL_SetWindowTitle(g_window, title_buffer);
        #endif // BUILD_DEBUG

        // The window starts out hidden, after the first draw we unhide the window as this looks quite clean.
        if(NK_CHECK_FLAGS(SDL_GetWindowFlags(g_window), SDL_WINDOW_HIDDEN))
        {
            SDL_ShowWindow(g_window);
        }
    }

    game_quit();

    SDL_GL_DeleteContext(g_glcontext);
    SDL_DestroyWindow(g_window);

    SDL_Quit();

    return 0;
}
