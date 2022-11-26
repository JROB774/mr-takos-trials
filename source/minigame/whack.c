/*////////////////////////////////////////////////////////////////////////////*/

#define WHACK_MIN_SPAWN_TIME 0.8f
#define WHACK_MAX_SPAWN_TIME 4.3f

#define WHACK_MIN_ALIVE_TIME 0.6f
#define WHACK_MAX_ALIVE_TIME 2.2f

#define WHACK_CHAR_COUNT 5

NK_ENUM(WhackCharState, nkS32)
{
    WhackCharState_Empty,
    WhackCharState_Grumble,
    WhackCharState_Blobo
};

static const nkVec4 WHACK_CHAR_BOUNDS[WHACK_CHAR_COUNT] =
{
    {  81.0f,  98.0f, 60.0f, 80.0f },
    { 239.0f,  98.0f, 60.0f, 80.0f },
    { 397.0f,  98.0f, 60.0f, 80.0f },
    { 160.0f, 163.0f, 60.0f, 80.0f },
    { 318.0f, 163.0f, 60.0f, 80.0f }
};

typedef struct MiniGameWhack
{
    WhackCharState state[WHACK_CHAR_COUNT];
    nkF32          timer[WHACK_CHAR_COUNT];
    nkS32          combo;
}
MiniGameWhack;

static MiniGameWhack g_minigame_whack;

static void minigame_whack_start(void)
{
    cursor_set_type(CursorType_Shovel);

    g_minigame_whack.combo = 0;

    for(nkS32 i=0; i<WHACK_CHAR_COUNT; ++i)
    {
        g_minigame_whack.state[i] = WhackCharState_Empty;
        g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_SPAWN_TIME,WHACK_MAX_SPAWN_TIME);
    }
}

static void minigame_whack_end(void)
{
    // Nothing...
}

static void minigame_whack_update(nkF32 dt)
{
    // @Incomplete: Randomly spawn and de-spawn creatures (faster as time goes on)...
    if(game_is_playing())
    {
        // Update all of the timers for spawning and de-spawning.
        for(nkS32 i=0; i<WHACK_CHAR_COUNT; ++i)
        {
            g_minigame_whack.timer[i] -= dt;
            if(g_minigame_whack.timer[i] <= 0.0f)
            {
                nkF32 speed_modifier = 0.0f;

                if(g_minigame_whack.state[i] == WhackCharState_Empty)
                {
                    g_minigame_whack.state[i] = rng_int_range(WhackCharState_Grumble,WhackCharState_Blobo);
                    g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_ALIVE_TIME,WHACK_MAX_ALIVE_TIME) - speed_modifier;
                }
                else
                {
                    g_minigame_whack.state[i] = WhackCharState_Empty;
                    g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_SPAWN_TIME,WHACK_MAX_SPAWN_TIME) - speed_modifier;
                }
            }
        }

        // Handle the user trying to hit something.
        if(is_mouse_button_pressed(MouseButton_Left) && !game_is_in_timeout())
        {
            sound_play(g_asset_sfx_shovel_whack, 0);

            // Check if we collided with any of the creatures.
            for(nkS32 i=0; i<WHACK_CHAR_COUNT; ++i)
            {
                if(g_minigame_whack.state[i] != WhackCharState_Empty)
                {
                    nkVec4 b = WHACK_CHAR_BOUNDS[i];

                    nkF32 x = b.x - (b.z * 0.5f);
                    nkF32 y = b.y - (b.w * 0.5f);
                    nkF32 w = b.z;
                    nkF32 h = b.w;

                    if(cursor_in_bounds(x,y,w,h))
                    {
                        sound_play(g_asset_sfx_smack[rng_int_range(0,2)], 0);
                        sound_play(g_asset_sfx_whack[rng_int_range(0,2)], 0);

                        if(g_minigame_whack.state[i] == WhackCharState_Grumble)
                        {
                            game_display_success((SCREEN_WIDTH * 0.5f) + 80.0f, (SCREEN_HEIGHT - 32.0f));
                            g_gamestate.game_score += 50 + (5 * g_minigame_whack.combo);
                            g_minigame_whack.combo++;
                        }
                        if(g_minigame_whack.state[i] == WhackCharState_Blobo)
                        {
                            game_display_failure((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));
                            g_minigame_whack.combo = 0;
                            g_gamestate.game_score -= 200;
                        }

                        g_minigame_whack.state[i] = WhackCharState_Empty;
                        g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_SPAWN_TIME,WHACK_MAX_SPAWN_TIME);
                    }
                }
            }
        }
    }
}

static void minigame_whack_render(void)
{
    // Draw the dirt mounds and the characters behind them (if there are any).
    imm_begin_texture_batch(g_asset_whack);
    for(nkS32 i=0; i<WHACK_CHAR_COUNT; ++i)
    {
        nkVec4 b = WHACK_CHAR_BOUNDS[i];

        nkS32 index = NK_S32_MAX;
        if(g_minigame_whack.state[i] == WhackCharState_Grumble) index = ATLAS_WHACK_GRUMBLE_BODY;
        if(g_minigame_whack.state[i] == WhackCharState_Blobo) index = ATLAS_WHACK_BLOBO_BODY;

        if(index != NK_S32_MAX)
        {
            nkF32 angle_a = g_angles_lil[i] * 1.5f;
            nkF32 angle_b = g_angles_lil[i+WHACK_CHAR_COUNT] * 0.5f;

            render_item_ex(b.x,b.y, 1,1, angle_a, ATLAS_WHACK, index, 1.0f);
            render_item_ex(b.x,b.y, 1,1, angle_b, ATLAS_WHACK, ATLAS_WHACK_DIRT_BODY, 1.0f);
        }
        else
        {
            render_item(b.x,b.y, ATLAS_WHACK, ATLAS_WHACK_DIRT_BODY, 1.0f);
        }
    }
    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
