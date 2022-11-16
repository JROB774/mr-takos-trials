/*////////////////////////////////////////////////////////////////////////////*/

NK_ENUM(SimonID, nkS32)
{
    SimonID_Ba,
    SimonID_Be,
    SimonID_Bo,
    SimonID_Bu,
    SimonID_TOTAL
};

typedef struct MiniGameSimon
{
    SimonID pattern[8];
    nkS32   pattern_length;
    nkS32   pattern_stage;
    nkS32   combo;
    nkF32   angle;
    nkF32   angle_timer;
}
MiniGameSimon;

static MiniGameSimon g_minigame_simon;

static void minigame_simon_generate_new_pattern(void)
{
    g_minigame_simon.pattern_length = rng_int_range(&g_rng_l, 0, NK_ARRAY_SIZE(g_minigame_simon.pattern)-1);
    g_minigame_simon.pattern_stage = 0;

    for(nkS32 i=0; i<g_minigame_simon.pattern_length; ++i)
    {
        g_minigame_simon.pattern[i] = rng_int_range(&g_rng_l, 0,SimonID_TOTAL-1);
    }
}

static void minigame_simon_init(void)
{
    // Nothing...
}

static void minigame_simon_quit(void)
{
    // Nothing...
}

static void minigame_simon_start(void)
{
    cursor_set_type(CursorType_Pointer);

    minigame_simon_generate_new_pattern();

    g_minigame_simon.combo = 0;
}

static void minigame_simon_end(void)
{
    // Nothing...
}

static void minigame_simon_update(nkF32 dt)
{
    // Update the render angle.
    g_minigame_simon.angle_timer += dt;
    if(g_minigame_simon.angle_timer >= ITEM_ANGLE_CHANGE_SPEED)
    {
        g_minigame_simon.angle_timer -= ITEM_ANGLE_CHANGE_SPEED;
        g_minigame_simon.angle = update_item_angle(g_minigame_simon.angle, -0.1f,0.1f);
    }
}

static void minigame_simon_render(void)
{
    // Draw the different characters.
    static const nkVec4 CHAR_BOUNDS[SimonID_TOTAL] =
    {
        {  70.0f, 152.0f,  80.0f, 110.0f },
        { 185.0f, 150.0f, 112.0f,  72.0f },
        { 335.0f,  95.0f, 145.0f, 112.0f },
        { 375.0f, 200.0f, 135.0f,  70.0f }
    };

    imm_begin_texture_batch(g_asset_game_simon);
    for(nkS32 i=0; i<SimonID_TOTAL; ++i)
    {
        nkVec4 b = CHAR_BOUNDS[i];

        nkF32 x = b.x;
        nkF32 y = b.y;

        // The pos is centered so need to make it top-left for collision.
        b.x -= b.z * 0.5f;
        b.y -= b.w * 0.5f;

        if(game_is_playing() && cursor_in_bounds(b.x,b.y,b.z,b.w))
        {
            render_item_ex(x,y, 1,1, g_minigame_simon.angle, ATLAS_GAMESIMON, (i*4)+1, 1.0f);
        }
        else
        {
            render_item(x,y, ATLAS_GAMESIMON, (i*4)+1, 1.0f);
        }
    }
    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
