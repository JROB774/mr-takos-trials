/*////////////////////////////////////////////////////////////////////////////*/

#define GLEW_STATIC
#define NK_STATIC

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <nk_define.h>
#include <nk_math.h>

#include <SDL.h>
#include <SDL_mixer.h>

#if defined(BUILD_NATIVE)
#include <glew.c>
#endif // BUILD_NATIVE

#if defined(BUILD_WEB)
#include <GLES2/gl2.h>
#endif // BUILD_WEB

#include "platform.h"
#include "render.h"
#include "game.h"

#include "render.c"
#include "game.c"

#define WINDOW_TITLE  "Tako Typing Teacher"
#define WINDOW_XPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_YPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_FLAGS  SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL

static SDL_Window*   g_window;
static SDL_GLContext g_glcontext;
static nkBool        g_running;

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

static void set_fullscreen(nkBool enable)
{
    SDL_SetWindowFullscreen(g_window, (enable) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    SDL_ShowCursor(get_fullscreen() ? SDL_DISABLE : SDL_ENABLE); // Hide the cursor in fullscreen mode.
}

static nkBool get_fullscreen(void)
{
    return NK_CHECK_FLAGS(SDL_GetWindowFlags(g_window), SDL_WINDOW_FULLSCREEN_DESKTOP);
}

static void main_init(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
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

    renderer_init();
    game_init();

    g_running = NK_TRUE;
}

static void main_quit(void)
{
    game_quit();
    renderer_quit();

    SDL_GL_DeleteContext(g_glcontext);
    SDL_DestroyWindow(g_window);

    SDL_Quit();
}

static void main_loop(void)
{
    static nkU64 perf_frequency = 0;
    static nkU64 last_counter = 0;
    static nkU64 end_counter = 0;
    static nkU64 elapsed_counter = 0;
    static nkF32 update_timer = 0.0f;

    static nkF32 dt = 1.0f / 60.0f; // We use a fixed update rate to keep things deterministic.

    if(perf_frequency == 0)
    {
        perf_frequency = SDL_GetPerformanceFrequency();
        last_counter = SDL_GetPerformanceCounter();
    }

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
                g_running = NK_FALSE;
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

// Different main function flow for native desktop and web builds.

#if defined(BUILD_NATIVE)
int main(int argc, char** argv)
{
    main_init();
    while(g_running)
        main_loop();
    main_quit();
    return 0;
}
#endif // BUILD_NATIVE

#if defined(BUILD_WEB)
#include <emscripten.h>
void main_callback(void)
{
    main_init();
    emscripten_set_main_loop(main_loop, -1, 1);
}
int main(int argc, char** argv)
{
    EM_ASM
    (
        FS.mkdir("/TAKO");
        FS.mount(IDBFS, {}, "/TAKO");
        FS.syncfs(true, function(err)
        {
            assert(!err);
            ccall("main_callback");
        });
    );
    return 0;
}
#endif // BUILD_WEB

/*////////////////////////////////////////////////////////////////////////////*/
