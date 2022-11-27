/*////////////////////////////////////////////////////////////////////////////*/

#define WHACK_MIN_SPAWN_TIME 0.8f
#define WHACK_MAX_SPAWN_TIME 3.5f

#define WHACK_MIN_ALIVE_TIME 0.6f
#define WHACK_MAX_ALIVE_TIME 1.3f

#define WHACK_CHAR_COUNT 5

#define WHACK_ANIM_SPEED  0.05f
#define WHACK_ANIM_LENGTH 2

#define WHACK_HITBOX_WIDTH  70.0f
#define WHACK_HITBOX_HEIGHT 80.0f

NK_ENUM(WhackCharState, nkS32)
{
    WhackCharState_Empty,
    WhackCharState_Grumble,
    WhackCharState_Blobo
};

static const nkVec4 WHACK_CHAR_BOUNDS[WHACK_CHAR_COUNT] =
{
    {  81.0f,  98.0f, WHACK_HITBOX_WIDTH, WHACK_HITBOX_HEIGHT },
    { 239.0f,  98.0f, WHACK_HITBOX_WIDTH, WHACK_HITBOX_HEIGHT },
    { 397.0f,  98.0f, WHACK_HITBOX_WIDTH, WHACK_HITBOX_HEIGHT },
    { 160.0f, 163.0f, WHACK_HITBOX_WIDTH, WHACK_HITBOX_HEIGHT },
    { 318.0f, 163.0f, WHACK_HITBOX_WIDTH, WHACK_HITBOX_HEIGHT }
};

typedef struct MiniGameWhack
{
    WhackCharState state[WHACK_CHAR_COUNT];
    nkF32          timer[WHACK_CHAR_COUNT];
    nkS32          frame[WHACK_CHAR_COUNT];
    nkF32          atime[WHACK_CHAR_COUNT];
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
        g_minigame_whack.frame[i] = 0;
        g_minigame_whack.atime[i] = 0.0f;
    }
}

static void minigame_whack_end(void)
{
    // Nothing...
}

static void minigame_whack_update(nkF32 dt)
{
    // Update animations for the creatures.
    for(nkS32 i=0; i<WHACK_CHAR_COUNT; ++i)
    {
        if((g_minigame_whack.state[i] != WhackCharState_Empty))
        {
            if(g_minigame_whack.timer[i] >= WHACK_ANIM_SPEED * 3.0f)
            {
                if(g_minigame_whack.frame[i] < WHACK_ANIM_LENGTH)
                {
                    g_minigame_whack.atime[i] += dt;
                    if(g_minigame_whack.atime[i] >= WHACK_ANIM_SPEED)
                    {
                        g_minigame_whack.atime[i] -= WHACK_ANIM_SPEED;
                        g_minigame_whack.frame[i]++;
                    }
                }
            }
            else
            {
                if(g_minigame_whack.frame[i] > 0)
                {
                    g_minigame_whack.atime[i] += dt;
                    if(g_minigame_whack.atime[i] >= WHACK_ANIM_SPEED)
                    {
                        g_minigame_whack.atime[i] -= WHACK_ANIM_SPEED;
                        g_minigame_whack.frame[i]--;
                    }
                }
            }
        }
    }

    if(game_is_playing())
    {
        // Update all of the timers for spawning and de-spawning.
        for(nkS32 i=0; i<WHACK_CHAR_COUNT; ++i)
        {
            g_minigame_whack.timer[i] -= dt;
            if(g_minigame_whack.timer[i] <= 0.0f)
            {
                sound_play(g_asset_sfx_crumple[rng_int_range(0,4)],0);

                if(g_minigame_whack.state[i] == WhackCharState_Empty)
                {
                    g_minigame_whack.state[i] = rng_int_range(WhackCharState_Grumble,WhackCharState_Blobo);
                    g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_ALIVE_TIME,WHACK_MAX_ALIVE_TIME);
                    g_minigame_whack.frame[i] = 0;
                    g_minigame_whack.atime[i] = 0.0f;
                }
                else
                {
                    g_minigame_whack.state[i] = WhackCharState_Empty;
                    g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_SPAWN_TIME,WHACK_MAX_SPAWN_TIME);
                }
            }
        }

        // Handle the user trying to hit something.
        if(is_mouse_button_pressed(MouseButton_Left) && !game_is_in_timeout())
        {
            sound_play(g_asset_sfx_shovel_whack, 0);

            nkBool hit_something = NK_FALSE;

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
                        if(g_minigame_whack.state[i] == WhackCharState_Grumble)
                        {
                            game_display_success((SCREEN_WIDTH * 0.5f) + 80.0f, (SCREEN_HEIGHT - 32.0f));
                            g_gamestate.game_score += 50 + (5 * g_minigame_whack.combo);
                            g_minigame_whack.combo++;
                            hit_something = NK_TRUE;
                        }
                        if(g_minigame_whack.state[i] == WhackCharState_Blobo)
                        {
                            game_display_failure((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));
                            g_minigame_whack.combo = 0;
                            g_gamestate.game_score -= 200;
                        }

                        g_minigame_whack.state[i] = WhackCharState_Empty;
                        g_minigame_whack.timer[i] = rng_num_range(WHACK_MIN_SPAWN_TIME,WHACK_MAX_SPAWN_TIME);

                        sound_play(g_asset_sfx_smack[rng_int_range(0,2)], 0);
                        sound_play(g_asset_sfx_whack[rng_int_range(0,2)], 0);

                        // x +=  5.0f;
                        // y +=  5.0f;
                        // w -= 10.0f;
                        // h -= 10.0f;

                        particles_spawn(ParticleType_Star, x,y,w,h, 3,5);
                    }
                }
            }

            // Drop the combo if the player misses.
            if(!hit_something)
            {
                g_minigame_whack.combo = 0;
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
        if(g_minigame_whack.state[i] == WhackCharState_Grumble) index = ATLAS_WHACK_GRUMBLE_SHADOW_0;
        if(g_minigame_whack.state[i] == WhackCharState_Blobo) index = ATLAS_WHACK_BLOBO_SHADOW_0;

        if(index != NK_S32_MAX)
        {
            nkF32 angle_a = g_angles_lil[i] * 1.5f;
            nkF32 angle_b = g_angles_lil[i+WHACK_CHAR_COUNT] * 0.5f;

            index += ((g_minigame_whack.frame[i] * 2) + 1);

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
