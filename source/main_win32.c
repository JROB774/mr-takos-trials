#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <nk_define.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>

#define WINDOW_TITLE  "Tako Typing Teacher"
#define WINDOW_XPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_YPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_FLAGS  SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL

static SDL_Window*   g_window;
static SDL_GLContext g_glcontext;

static void fatal_error(const char* fmt, ...)
{
    char message_buffer[1024] = NK_ZERO_MEM;

    va_list args;

    va_start(args, fmt);
    vsnprintf(message_buffer, NK_ARRAY_SIZE(message_buffer), fmt, args);
    va_end(args);

    #if defined(BUILD_DEBUG)
    fprintf(stderr, "%s\n", message_buffer);
    #endif

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Fatal Error", message_buffer, g_window);

    abort();
}

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fatal_error("Failed to initialize SDL systems: %s", SDL_GetError());
    }

    g_window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_XPOS,WINDOW_YPOS,
        WINDOW_WIDTH,WINDOW_HEIGHT, WINDOW_FLAGS);
    if(!g_window)
    {
        fatal_error("Failed to create application window: %s", SDL_GetError());
    }

    g_glcontext = SDL_GL_CreateContext(g_window);
    if(!g_glcontext)
    {
        fatal_error("Failed to create OpenGL context: %s", SDL_GetError());
    }

    SDL_ShowWindow(g_window);

    nkBool running = NK_TRUE;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = NK_FALSE;
            }
        }

        glClearColor(1,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(g_window);
    }

    SDL_GL_DeleteContext(g_glcontext);
    SDL_DestroyWindow(g_window);

    SDL_Quit();

    return 0;
}
