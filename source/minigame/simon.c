/*////////////////////////////////////////////////////////////////////////////*/

#define SIMON_MIN_PATTERN_LENGTH 2
#define SIMON_MAX_PATTERN_LENGTH 5

#define SIMON_PATTERN_INTERVAL 0.4f
#define SIMON_ANIM_INTERVAL    0.3f

#define SIMON_INITIAL_COOLDOWN 0.2f

NK_ENUM(SimonID, nkS32)
{
    SimonID_Ba,
    SimonID_Be,
    SimonID_Bo,
    SimonID_Bu,
    SimonID_TOTAL
};

static const nkVec4 SIMON_CHAR_BOUNDS[SimonID_TOTAL] =
{
    {  70.0f, 152.0f,  80.0f, 110.0f },
    { 185.0f, 150.0f, 112.0f,  72.0f },
    { 335.0f,  95.0f, 145.0f, 112.0f },
    { 375.0f, 200.0f, 135.0f,  70.0f }
};

typedef struct MiniGameSimon
{
    SimonID pattern[SIMON_MAX_PATTERN_LENGTH];
    nkS32   pattern_length;
    nkS32   pattern_stage;
    nkS32   combo;
    nkF32   anim_timer;
    nkF32   initial_timer;
    nkS32   selected;
    nkS32   playback_stage;
    nkF32   playback_timer;
    nkBool  playback_pattern;
}
MiniGameSimon;

static MiniGameSimon g_minigame_simon;

static void minigame_simon_generate_new_pattern(void)
{
    g_minigame_simon.pattern_length = rng_int_range(SIMON_MIN_PATTERN_LENGTH,SIMON_MAX_PATTERN_LENGTH);
    g_minigame_simon.pattern_stage = 0;

    g_minigame_simon.playback_stage = 0;
    g_minigame_simon.playback_timer = 0.0f;
    g_minigame_simon.playback_pattern = NK_TRUE;

    for(nkS32 i=0; i<g_minigame_simon.pattern_length; ++i)
    {
        g_minigame_simon.pattern[i] = rng_int_range(0,SimonID_TOTAL-1);
    }
}

static void minigame_simon_start(void)
{
    cursor_set_type(CursorType_Pointer);

    minigame_simon_generate_new_pattern();

    g_minigame_simon.combo = 0;
    g_minigame_simon.selected = -1;

    g_minigame_simon.initial_timer = SIMON_INITIAL_COOLDOWN;
}

static void minigame_simon_end(void)
{
    // Nothing...
}

static void minigame_simon_update(nkF32 dt)
{
    // Update the animation timer.
    g_minigame_simon.anim_timer += dt;
    if(g_minigame_simon.anim_timer >= SIMON_ANIM_INTERVAL)
    {
        g_minigame_simon.selected = -1;
    }

    // Update game logic.
    if(game_is_playing())
    {
        if(g_minigame_simon.playback_pattern)
        {
            if(g_minigame_simon.initial_timer <= 0.0f)
            {
                // Playback the pattern to the user.
                g_minigame_simon.playback_timer += dt;
                if(g_minigame_simon.playback_timer >= SIMON_PATTERN_INTERVAL)
                {
                    g_minigame_simon.playback_timer -= SIMON_PATTERN_INTERVAL;
                    if(g_minigame_simon.playback_stage >= g_minigame_simon.pattern_length)
                    {
                        g_minigame_simon.playback_pattern = NK_FALSE;
                    }
                    else
                    {
                        nkS32 current = g_minigame_simon.pattern[g_minigame_simon.playback_stage];
                        sound_play(g_asset_sfx_simon[current], 0);

                        g_minigame_simon.playback_stage++;
                        g_minigame_simon.selected = current;
                        g_minigame_simon.anim_timer = 0.0f;
                    }
                }
            }
            else
            {
                g_minigame_simon.initial_timer -= dt;
            }
        }
        else
        {
            // Let the user select a creature.
            for(nkS32 i=0; i<SimonID_TOTAL; ++i)
            {
                nkVec4 b = SIMON_CHAR_BOUNDS[i];

                // The pos is centered so need to make it top-left for collision.
                b.x -= b.z * 0.5f;
                b.y -= b.w * 0.5f;

                if(cursor_in_bounds(b.x,b.y,b.z,b.w) && is_mouse_button_pressed(MouseButton_Left) && !game_is_in_timeout())
                {
                    sound_play(g_asset_sfx_simon[i], 0);
                    g_minigame_simon.selected = i;
                    g_minigame_simon.anim_timer = 0.0f;

                    nkBool correct = (i == g_minigame_simon.pattern[g_minigame_simon.pattern_stage]);
                    if(correct)
                    {
                        g_minigame_simon.pattern_stage++;
                        g_minigame_simon.combo++;

                        g_gamestate.game_score += 5 * g_minigame_simon.combo;

                        if(g_minigame_simon.pattern_stage >= g_minigame_simon.pattern_length)
                        {
                            game_display_success((SCREEN_WIDTH * 0.5f) + 80.0f, (SCREEN_HEIGHT - 32.0f));
                            g_minigame_simon.initial_timer = SIMON_INITIAL_COOLDOWN;

                            g_gamestate.game_score += (g_minigame_simon.pattern_length * 100); // Pattern bonus!

                            minigame_simon_generate_new_pattern();
                        }
                    }
                    else
                    {
                        game_display_failure((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));
                        g_minigame_simon.initial_timer = GAME_FAILURE_COUNTDOWN;

                        g_minigame_typer.combo = 0;
                        g_gamestate.game_score -= 200;

                        minigame_simon_generate_new_pattern();
                    }
                }
            }
        }
    }
}

static void minigame_simon_render(void)
{
    // Draw the different characters.
    imm_begin_texture_batch(g_asset_simon);
    for(nkS32 i=0; i<SimonID_TOTAL; ++i)
    {
        nkVec4 b = SIMON_CHAR_BOUNDS[i];

        nkF32 x = b.x;
        nkF32 y = b.y;

        // The pos is centered so need to make it top-left for collision.
        b.x -= b.z * 0.5f;
        b.y -= b.w * 0.5f;

        // Potentially play the singing animation.
        nkS32 index = ((g_minigame_simon.selected == i) ? (i*4)+3 : (i*4)+1);

        if(game_is_playing() && !g_minigame_simon.playback_pattern && cursor_in_bounds(b.x,b.y,b.z,b.w) && !game_is_in_timeout())
        {
            render_item_ex(x,y, 1,1, g_angles_lil[0], ATLAS_SIMON, index, 1.0f);
        }
        else
        {
            render_item(x,y, ATLAS_SIMON, index, 1.0f);
        }
    }
    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
