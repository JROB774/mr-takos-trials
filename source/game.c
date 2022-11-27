/*////////////////////////////////////////////////////////////////////////////*/

#define GAME_SUCCESS_COUNTDOWN 0.8f
#define GAME_FAILURE_COUNTDOWN 0.8f

typedef struct GameState
{
    MiniGameID current_minigame;
    nkF32      intro_timer;
    nkF32      game_timer;
    nkS32      game_score;
    nkU64      end_state;
    nkF32      success_countdown;
    nkF32      failure_countdown;
    nkVec2     success_pos;
    nkVec2     failure_pos;
    SoundRef   tick_sound_ref;
    SoundRef   alarm_sound_ref;
}
GameState;

static GameState g_gamestate;

#include "minigame/typer.c"
#include "minigame/simon.c"
#include "minigame/whack.c"
#include "minigame/match.c"

typedef void(*MiniGameHook_Start )(void );
typedef void(*MiniGameHook_End   )(void );
typedef void(*MiniGameHook_Update)(nkF32);
typedef void(*MiniGameHook_Render)(void );

typedef struct MiniGameHooks
{
    MiniGameHook_Start  start;
    MiniGameHook_End    end;
    MiniGameHook_Update update;
    MiniGameHook_Render render;
}
MiniGameHooks;

#define REGISTER_MINIGAME(name) \
{                               \
    minigame_##name##_start,    \
    minigame_##name##_end,      \
    minigame_##name##_update,   \
    minigame_##name##_render    \
}

static const MiniGameHooks MINI_GAME_HOOKS[] =
{
    REGISTER_MINIGAME(typer),
    REGISTER_MINIGAME(simon),
    REGISTER_MINIGAME(whack),
    REGISTER_MINIGAME(match),
};

NK_STATIC_ASSERT(MiniGameID_TOTAL == NK_ARRAY_SIZE(MINI_GAME_HOOKS), minigame_size_mismatch);

static void game_init(void)
{
    // Nothing...
}

static void game_quit(void)
{
    // Nothing...
}

static void game_start(MiniGameID minigame)
{
    g_gamestate.current_minigame = minigame;

    g_gamestate.intro_timer = 3.25f;
    g_gamestate.game_timer = 20.0f;

    g_gamestate.game_score = 0;

    g_gamestate.end_state = 0;

    g_gamestate.success_countdown = 0.0f;
    g_gamestate.failure_countdown = 0.0f;

    g_gamestate.tick_sound_ref = INVALID_SOUND_REF;
    g_gamestate.alarm_sound_ref = INVALID_SOUND_REF;

    MINI_GAME_HOOKS[g_gamestate.current_minigame].start();
}

static void game_end(void)
{
    g_gamestate.tick_sound_ref = INVALID_SOUND_REF;
    g_gamestate.alarm_sound_ref = sound_play(g_asset_sfx_alarm_clock, -1);

    MINI_GAME_HOOKS[g_gamestate.current_minigame].end();

    if(g_gamestate.game_score > g_save.highscore[g_gamestate.current_minigame])
    {
        g_save.highscore[g_gamestate.current_minigame] = g_gamestate.game_score;
        save_game_data();
    }
}

static void game_update(nkF32 dt)
{
    // Update the current game.
    MINI_GAME_HOOKS[g_gamestate.current_minigame].update(dt);

    // Keep the game score in bounds.
    if(g_gamestate.game_score < 0)
    {
        g_gamestate.game_score = 0;
    }

    // Decrement the success/failure timers.
    if(g_gamestate.success_countdown > 0.0f)
    {
        g_gamestate.success_countdown -= dt;
    }
    if(g_gamestate.failure_countdown > 0.0f)
    {
        g_gamestate.failure_countdown -= dt;
    }

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
            g_gamestate.tick_sound_ref = sound_play(g_asset_sfx_clock_ticking, 0);
        }
        if(prev_timer > 0.0f && g_gamestate.game_timer <= 0.0f)
        {
            game_end();
        }

        // Do the end game flow...
        if(g_gamestate.game_timer <= 0.0f)
        {
            if(is_mouse_button_pressed(MouseButton_Left))
            {
                g_gamestate.end_state++;

                // On the first click fade out the alarm and display high score effects if there was one.
                if(g_gamestate.end_state == 1)
                {
                    sound_fade_out(g_gamestate.alarm_sound_ref, 1.0f);
                    g_gamestate.alarm_sound_ref = INVALID_SOUND_REF;

                    if(g_save.highscore[g_gamestate.current_minigame] <= g_gamestate.game_score)
                    {
                        sound_play(g_asset_sfx_trumpet_fanfare, 0);
                    }
                }

                // On the second click return to the main menu.
                if(g_gamestate.end_state == 2)
                {
                    g_appstate = AppState_Menu;
                    change_page();
                }
            }
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

    nkF32 ox = (g_gamestate.game_timer <= 0.0f) ? rng_int_range(-1,1) : 0.0f;
    nkF32 oy = (g_gamestate.game_timer <= 0.0f) ? rng_int_range(-1,1) : 0.0f;

    x -= (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.w * 0.5f) + PADDING;
    y += (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.h * 0.5f);

    render_item(x+ox,y+oy, ATLAS_UI, ATLAS_UI_CLOCK_BODY, 1.0f);

    x += (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.w * 0.5f) + PADDING;

    nkChar timer_buffer[8] = NK_ZERO_MEM;
    sprintf(timer_buffer, "%04.1f0", g_gamestate.game_timer);

    for(nkU32 i=0,n=strlen(timer_buffer); i<n; ++i)
    {
        ox = (g_gamestate.game_timer <= DANGER_TIME) ? rng_int_range(-1,1) : 0.0f;
        oy = (g_gamestate.game_timer <= DANGER_TIME) ? rng_int_range(-1,1) : 0.0f;

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

    // Render the score.
    nkChar score_buffer[16] = NK_ZERO_MEM;
    sprintf(score_buffer, "%05d", g_gamestate.game_score);

    x = (SCREEN_WIDTH - (LETTER_WIDTH*strlen(score_buffer))) * 0.5f;
    y = SCREEN_HEIGHT - 32.0f;

    for(nkU32 i=0,n=strlen(score_buffer); i<n; ++i)
    {
        nkS32 index = ATLAS_UI_TIMER_0_SHADOW + (((score_buffer[i] - '0') * 2) + 1);
        x += LETTER_WIDTH * 0.5f;
        render_item_ex(x,y, 0.7f,0.7f, 0.0f, ATLAS_UI, index, 0.7f);
        x += LETTER_WIDTH * 0.5f;
    }

    // If this score is a highscore then draw a cool crown.
    if(g_gamestate.game_timer <= 0.0f && g_gamestate.end_state >= 1)
    {
        if(g_save.highscore[g_gamestate.current_minigame] <= g_gamestate.game_score)
        {
            render_item_ex(x+3.0f,y-23.0f, 1,1, 0.4f, ATLAS_UI, ATLAS_UI_CROWN_BODY, 0.7f);
        }
    }

    // Draw the happy face if the player was successful.
    if(g_gamestate.success_countdown > 0.0f)
    {
        nkF32 x = g_gamestate.success_pos.x;
        nkF32 y = g_gamestate.success_pos.y;

        render_item_ex(x,y, 0.8f,0.8f, g_angles_big[APP_MAX_ANGLES-2], ATLAS_UI, ATLAS_UI_FEEDBACK_HAPPY_BODY, 1.0f);
    }

    // Draw the sad face if the player is on timeout.
    if(g_gamestate.failure_countdown > 0.0f)
    {
        nkF32 x = g_gamestate.failure_pos.x;
        nkF32 y = g_gamestate.failure_pos.y;

        render_item_ex(x,y, 1,1, g_angles_big[APP_MAX_ANGLES-1], ATLAS_UI, ATLAS_UI_FEEDBACK_SAD_BODY, 1.0f);
    }

    imm_end_texture_batch();
}

static void game_display_success(nkF32 x, nkF32 y)
{
    g_gamestate.success_countdown = GAME_SUCCESS_COUNTDOWN;
    g_gamestate.success_pos = (nkVec2){ x,y };

    sound_play(g_asset_sfx_success_ding, 0);
}

static void game_display_failure(nkF32 x, nkF32 y)
{
    g_gamestate.failure_countdown = GAME_FAILURE_COUNTDOWN;
    g_gamestate.failure_pos = (nkVec2){ x,y };

    sound_play(g_asset_sfx_wrong_buzzer, 0);
}

static nkF32 game_get_timer(void)
{
    return g_gamestate.game_timer;
}

static nkBool game_is_playing(void)
{
    return ((g_gamestate.game_timer > 0.0f) && (g_gamestate.intro_timer == 0.0f));
}

static nkBool game_is_in_timeout(void)
{
    return (g_gamestate.failure_countdown > 0.0f);
}

static nkBool game_is_in_countdown(void)
{
    return (g_gamestate.intro_timer > 0.0f);
}

/*////////////////////////////////////////////////////////////////////////////*/
