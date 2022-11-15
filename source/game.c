/*////////////////////////////////////////////////////////////////////////////*/

#include "minigame/typer.c"
#include "minigame/simon.c"

typedef void(*MiniGameHook_Init  )(void );
typedef void(*MiniGameHook_Quit  )(void );
typedef void(*MiniGameHook_Start )(void );
typedef void(*MiniGameHook_End   )(void );
typedef void(*MiniGameHook_Update)(nkF32);
typedef void(*MiniGameHook_Render)(void );

typedef struct MiniGameHooks
{
    MiniGameHook_Init   init;
    MiniGameHook_Quit   quit;
    MiniGameHook_Start  start;
    MiniGameHook_End    end;
    MiniGameHook_Update update;
    MiniGameHook_Render render;
}
MiniGameHooks;

NK_ENUM(MiniGameID, nkS32)
{
    MiniGameID_Typer,
    MiniGameID_Simon,
    MiniGameID_TOTAL
};

#define REGISTER_MINIGAME(name) \
{                               \
    minigame_##name##_init,     \
    minigame_##name##_quit,     \
    minigame_##name##_start,    \
    minigame_##name##_end,      \
    minigame_##name##_update,   \
    minigame_##name##_render    \
}

static const MiniGameHooks MINI_GAME_HOOKS[] =
{
    REGISTER_MINIGAME(typer),
    REGISTER_MINIGAME(simon)
};

NK_STATIC_ASSERT(MiniGameID_TOTAL == NK_ARRAY_SIZE(MINI_GAME_HOOKS), minigame_size_mismatch);

typedef struct GameState
{
    MiniGameID current_minigame;
    nkF32      intro_timer;
    nkF32      game_timer;
    nkBool     in_intro;
}
GameState;

static GameState g_gamestate;

static void game_start(void)
{
    g_gamestate.current_minigame = MiniGameID_Simon;

    g_gamestate.intro_timer = 3.25f;
    g_gamestate.game_timer = 20.0f;

    MINI_GAME_HOOKS[g_gamestate.current_minigame].start();
}

static void game_init(void)
{
    for(nkS32 i=0; i<MiniGameID_TOTAL; ++i)
    {
        MINI_GAME_HOOKS[i].init();
    }
}

static void game_quit(void)
{
    for(nkS32 i=0; i<MiniGameID_TOTAL; ++i)
    {
        MINI_GAME_HOOKS[i].quit();
    }
}

static void game_update(nkF32 dt)
{
    // Update the current game.
    MINI_GAME_HOOKS[g_gamestate.current_minigame].update(dt);

    if(g_gamestate.intro_timer > 0.0f)
    {
        // Perform the countdown.
        nkF32 prev_timer = g_gamestate.intro_timer;
        g_gamestate.intro_timer -= dt;
        if(g_gamestate.intro_timer < 0.0f)
        {
            g_gamestate.intro_timer = 0.0f;
        }

        // Play appropriate sounds.
        if(g_gamestate.intro_timer == 0.0f)
        {
            sound_play(g_asset_sfx_airhorn, 0);
        }
        else if(floorf(prev_timer) != floorf(g_gamestate.intro_timer))
        {
            nkS32 sound_index = NK_CAST(nkS32,floorf(g_gamestate.intro_timer)) % 3;
            sound_play(g_asset_sfx_smack[sound_index], 0);
        }
    }
    else
    {
        // Update the timer.
        nkF32 prev_timer = g_gamestate.game_timer;
        g_gamestate.game_timer -= dt;
        if(g_gamestate.game_timer < 0.0f)
        {
            g_gamestate.game_timer = 0.0f;
        }

        // Do the end game logic.
        if(prev_timer > 5.0f && g_gamestate.game_timer <= 5.0f)
        {
            sound_play(g_asset_sfx_clock_ticking, 0);
        }
        if(prev_timer > 0.0f && g_gamestate.game_timer <= 0.0f)
        {
            sound_play(g_asset_sfx_alarm_clock, 0);
            MINI_GAME_HOOKS[g_gamestate.current_minigame].end();
        }
    }
}

static void game_render(void)
{
    // Render the current game.
    MINI_GAME_HOOKS[g_gamestate.current_minigame].render();

    imm_begin_texture_batch(g_asset_ui);

    // Render the timer graphics and its effects.
    static const nkF32 LETTER_WIDTH = 15.0f;
    static const nkF32 PADDING = 4.0f;
    static const nkF32 DANGER_TIME = 5.0f;

    nkF32 width = LETTER_WIDTH * 5.0f;

    nkF32 x = (SCREEN_WIDTH - width) * 0.5f;
    nkF32 y = PADDING;

    nkF32 ox = (g_gamestate.game_timer <= 0.0f) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;
    nkF32 oy = (g_gamestate.game_timer <= 0.0f) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;

    x -= (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.w * 0.5f) + PADDING;
    y += (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.h * 0.5f);

    render_item(x+ox,y+oy, ATLAS_UI, ATLAS_UI_CLOCK_BODY, 1.0f);

    x += (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.w * 0.5f) + PADDING;

    nkChar timer_buffer[8] = NK_ZERO_MEM;
    sprintf(timer_buffer, "%04.1f0", g_gamestate.game_timer);

    for(nkU32 i=0,n=strlen(timer_buffer); i<n; ++i)
    {
        ox = (g_gamestate.game_timer <= DANGER_TIME) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;
        oy = (g_gamestate.game_timer <= DANGER_TIME) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;

        nkS32 index = ATLAS_UI_TIMER_0_SHADOW + (((timer_buffer[i] - '0') * 2) + 1);
        if(timer_buffer[i] == '.') index = ATLAS_UI_TIMER_DOT_BODY;

        x += LETTER_WIDTH * 0.5f;
        render_item_ex(x+ox,y+oy, 0.7f,0.7f, 0.0f, ATLAS_UI, index, 0.7f);
        x += LETTER_WIDTH * 0.5f;
    }

    // Render the countdown.
    if(g_gamestate.intro_timer > 0.0f)
    {
        nkS32 countdown = floorf(g_gamestate.intro_timer);
        nkS32 index = NK_S32_MAX;
        switch(countdown)
        {
            case 2: index = ATLAS_UI_COUNTDOWN_3_BODY; break;
            case 1: index = ATLAS_UI_COUNTDOWN_2_BODY; break;
            case 0: index = ATLAS_UI_COUNTDOWN_1_BODY; break;
        }
        if(index != NK_S32_MAX)
        {
            nkF32 x = SCREEN_WIDTH * 0.5f;
            nkF32 y = SCREEN_HEIGHT * 0.5f;

            render_item(x,y, ATLAS_UI, index, 1.0f);
        }
    }

    imm_end_texture_batch();
}

static nkF32 game_get_timer(void)
{
    return g_gamestate.game_timer;
}

static nkBool game_is_playing(void)
{
    return ((g_gamestate.game_timer > 0.0f) && (g_gamestate.intro_timer == 0.0f));
}

/*////////////////////////////////////////////////////////////////////////////*/
