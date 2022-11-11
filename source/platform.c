/*////////////////////////////////////////////////////////////////////////////*/

#define STB_IMAGE_STATIC
#define STBTT_STATIC
#define GLEW_STATIC
#define NK_STATIC

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define NK_MATH_IMPLEMENTATION
#define NK_FILESYS_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <float.h>

#include <nk_define.h>
#include <nk_math.h>
#include <nk_filesys.h>

#include <stb_image.h>
#include <stb_truetype.h>

#include <SDL.h>
#include <SDL_mixer.h>

#if defined(BUILD_NATIVE)
#include <glew.c>
#endif // BUILD_NATIVE
#if defined(BUILD_WEB)
#include <GLES2/gl2.h>
#endif // BUILD_WEB

#define WINDOW_TITLE  "TAKO TYPING TRIALS"
#define WINDOW_XPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_YPOS   SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH  1920 // @Incomplete: Change to be something smalle...
#define WINDOW_HEIGHT 1080 // @Incomplete: Change to be something smalle...
#define WINDOW_FLAGS  SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 270

#include "audio.h"
#include "game.h"
#include "render.h"
#include "immdraw.h"
#include "postprocess.h"
#include "platform.h"
#include "font.h"
#include "input.h"
#include "assets.h"

#include "codegen/atlas_letter.h"
#include "codegen/atlas_tako.h"

#include "audio.c"
#include "game.c"
#include "render.c"
#include "immdraw.c"
#include "postprocess.c"
#include "font.c"
#include "input.c"
#include "assets.c"

typedef struct PlatformContext
{
    SDL_Window*   window;
    SDL_GLContext glcontext;
    nkBool        running;
    RenderTarget  screentarget;
    VertexBuffer  screenbuffer;
    Shader        screenshader;
    nkChar*       base_path;
}
PlatformContext;

static PlatformContext g_ctx;

static const nkChar* get_base_path(void)
{
    return g_ctx.base_path;
}

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

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", message_buffer, g_ctx.window);

    abort();
}

static nkS32 window_get_width(void)
{
    nkS32 width;
    SDL_GetWindowSize(g_ctx.window, &width,NULL);
    return width;
}

static nkS32 window_get_height(void)
{
    nkS32 height;
    SDL_GetWindowSize(g_ctx.window, NULL,&height);
    return height;
}

static void set_fullscreen(nkBool enable)
{
    SDL_SetWindowFullscreen(g_ctx.window, (enable) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    SDL_ShowCursor(get_fullscreen() ? SDL_DISABLE : SDL_ENABLE); // Hide the cursor in fullscreen mode.
}

static nkBool get_fullscreen(void)
{
    return NK_CHECK_FLAGS(SDL_GetWindowFlags(g_ctx.window), SDL_WINDOW_FULLSCREEN_DESKTOP);
}

static void show_cursor(nkBool show)
{
    SDL_ShowCursor((show ? SDL_ENABLE : SDL_DISABLE));
}

static RenderTarget get_screen(void)
{
    return g_ctx.screentarget;
}

static void begin_render_frame(void)
{
    nkF32 vx = 0.0f;
    nkF32 vy = 0.0f;
    nkF32 vw = texture_get_width(g_ctx.screentarget->color_target);
    nkF32 vh = texture_get_height(g_ctx.screentarget->color_target);

    render_target_bind(g_ctx.screentarget);
    set_viewport(vx,vy,vw,vh);

    imm_set_projection(nk_orthographic(0,vw,vh,0,0,1));
    imm_set_view(nk_m4_identity());
    imm_set_model(nk_m4_identity());
}

static void end_render_frame(void)
{
    RenderTarget output = postprocess_get_result();

    render_target_bind(NULL);

    nkF32 ww = NK_CAST(nkF32, window_get_width());
    nkF32 wh = NK_CAST(nkF32, window_get_height());

    set_viewport(0.0f,0.0f,ww,wh);

    clear_screen_f(0.0f,0.0f,0.0f,1.0f);

    nkF32 dstw = SCREEN_WIDTH;
    nkF32 dsth = SCREEN_HEIGHT;

    while((dstw+SCREEN_WIDTH <= ww) && (dsth+SCREEN_HEIGHT <= wh))
    {
        dstw += SCREEN_WIDTH;
        dsth += SCREEN_HEIGHT;
    }

    nkF32 dstx0 = (ww-dstw)*0.5f;
    nkF32 dsty0 = (wh-dsth)*0.5f;
    nkF32 dstx1 = dstx0+dstw;
    nkF32 dsty1 = dsty0+dsth;

    nkVec4 vertices[4];

    vertices[0] = (nkVec4){ dstx0,dsty1, 0.0f,0.0f };
    vertices[1] = (nkVec4){ dstx0,dsty0, 0.0f,1.0f };
    vertices[2] = (nkVec4){ dstx1,dsty1, 1.0f,0.0f };
    vertices[3] = (nkVec4){ dstx1,dsty0, 1.0f,1.0f };

    nkMat4 projection = nk_orthographic(0,ww,wh,0,0,1);

    set_blend_mode(BlendMode_PremultipliedAlpha);

    texture_bind(output->color_target, 0);
    shader_bind(g_ctx.screenshader);

    shader_set_mat4(g_ctx.screenshader, "u_projection", projection);

    vertex_buffer_update(g_ctx.screenbuffer, vertices, sizeof(vertices), BufferType_Dynamic);
    vertex_buffer_draw(g_ctx.screenbuffer, DrawMode_TriangleStrip, NK_ARRAY_SIZE(vertices));
}

static void main_init(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fatal_error("Failed to initialize SDL systems: %s", SDL_GetError());
    }

    #if defined(BUILD_NATIVE)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    #endif // BUILD_NATIVE
    #if defined(BUILD_WEB)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #endif // BUILD_WEB

    g_ctx.window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_XPOS,WINDOW_YPOS, WINDOW_WIDTH,WINDOW_HEIGHT, WINDOW_FLAGS);
    if(!g_ctx.window)
        fatal_error("Failed to create application window: %s", SDL_GetError());
    SDL_SetWindowMinimumSize(g_ctx.window, SCREEN_WIDTH,SCREEN_HEIGHT);

    g_ctx.glcontext = SDL_GL_CreateContext(g_ctx.window);
    if(!g_ctx.glcontext)
    {
        fatal_error("Failed to create OpenGL context: %s", SDL_GetError());
    }

    g_ctx.base_path = SDL_GetBasePath();

    // Enable VSync by default, if we don't get it then oh well.
    if(SDL_GL_SetSwapInterval(1) == 0)
    {
        printf("VSync Enabled!\n");
    }

    renderer_init();
    imm_init();
    font_init();
    audio_init();

    g_ctx.screentarget = render_target_create(SCREEN_WIDTH,SCREEN_HEIGHT, SamplerFilter_Nearest, SamplerWrap_Clamp);
    g_ctx.screenshader = load_asset_shader("copy.shader");
    g_ctx.screenbuffer = vertex_buffer_create();

    vertex_buffer_set_stride   (g_ctx.screenbuffer, sizeof(nkF32)*4);
    vertex_buffer_enable_attrib(g_ctx.screenbuffer, 0, AttribType_Float, 4, 0);

    postprocess_init();

    game_init();

    g_ctx.running = NK_TRUE;
}

static void main_quit(void)
{
    game_quit();

    postprocess_quit();

    vertex_buffer_destroy(g_ctx.screenbuffer);
    shader_destroy(g_ctx.screenshader);
    render_target_destroy(g_ctx.screentarget);

    audio_quit();
    font_quit();
    imm_quit();
    renderer_quit();

    SDL_free(g_ctx.base_path);

    SDL_GL_DeleteContext(g_ctx.glcontext);
    SDL_DestroyWindow(g_ctx.window);

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
                g_ctx.running = NK_FALSE;
            } break;
        }
    }

    nkBool did_update = NK_FALSE;
    while(update_timer >= dt)
    {
        did_update = NK_TRUE;
        update_input_state();
        game_update(dt);
        update_timer -= dt;
    }
    if(did_update)
    {
        begin_render_frame();
        game_render();
        postprocess_execute();
    }
    end_render_frame();

    SDL_GL_SwapWindow(g_ctx.window);

    end_counter = SDL_GetPerformanceCounter();
    elapsed_counter = end_counter - last_counter;
    last_counter = SDL_GetPerformanceCounter();

    nkF32 elapsed_time = NK_CAST(nkF32,elapsed_counter) / NK_CAST(nkF32,perf_frequency);

    update_timer += elapsed_time;

    #if defined(BUILD_DEBUG)
    nkF32 current_fps = NK_CAST(nkF32,perf_frequency) / NK_CAST(nkF32,elapsed_counter);
    nkChar title_buffer[1024] = NK_ZERO_MEM;
    snprintf(title_buffer, NK_ARRAY_SIZE(title_buffer), "%s (FPS: %f)", WINDOW_TITLE, current_fps);
    SDL_SetWindowTitle(g_ctx.window, title_buffer);
    #endif // BUILD_DEBUG

    // The window starts out hidden, after the first draw we unhide the window as this looks quite clean.
    if(NK_CHECK_FLAGS(SDL_GetWindowFlags(g_ctx.window), SDL_WINDOW_HIDDEN))
    {
        SDL_ShowWindow(g_ctx.window);
    }
}

// Different main function flow for native desktop and web builds.

#if defined(BUILD_NATIVE)
int main(int argc, char** argv)
{
    main_init();
    while(g_ctx.running)
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
