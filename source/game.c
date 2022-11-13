/*////////////////////////////////////////////////////////////////////////////*/

NK_ENUM(MiniGame, nkS32)
{
    MiniGame_Typer,
    MiniGame_TOTAL
};

typedef struct GameState
{
    MiniGame minigame;
    nkF32    timer;
}
GameState;

static GameState g_state;

static void render_item(nkF32 x, nkF32 y, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 alpha)
{
    nkF32 sx = x + SHADOW_OFFSET.x;
    nkF32 sy = y + SHADOW_OFFSET.y;

    nkVec4 shadow_color = SHADOW_COLOR;
    nkVec4 body_color = BODY_COLOR;

    shadow_color.a *= alpha;
    body_color.a *= alpha;

    imm_atlas_batched(sx,sy, &atlas_clips[atlas_clip_index-1], shadow_color);
    imm_atlas_batched(x,y, &atlas_clips[atlas_clip_index], body_color);
}

static void render_item_ex(nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 alpha)
{
    nkF32 xs = x + (SHADOW_OFFSET.x * sx);
    nkF32 ys = y + (SHADOW_OFFSET.y * sy);

    nkVec4 shadow_color = SHADOW_COLOR;
    nkVec4 body_color = BODY_COLOR;

    shadow_color.a *= alpha;
    body_color.a *= alpha;

    imm_atlas_batched_ex(xs,ys, sx,sy, angle, NULL, &atlas_clips[atlas_clip_index-1], shadow_color);
    imm_atlas_batched_ex(x,y, sx,sy, angle, NULL, &atlas_clips[atlas_clip_index], body_color);
}

static void game_init(void)
{
    g_state.minigame = MiniGame_Typer;

    load_all_assets();

    nkU32 seed = time(NULL);

    rng_init(&g_rng_l, seed);
    rng_init(&g_rng_v, seed);

    show_cursor(NK_FALSE);

    minigame_typer_init();

    g_state.timer = 20.0f;
}

static void game_quit(void)
{
    minigame_typer_quit();

    free_all_assets();
}

static void game_update(nkF32 dt)
{
    switch(g_state.minigame)
    {
        case MiniGame_Typer: minigame_typer_update(dt); break;
    }

    nkF32 prev_timer = g_state.timer;
    g_state.timer -= dt;
    if(g_state.timer < 0.0f)
        g_state.timer = 0.0f;
    if(prev_timer > 0.0f && g_state.timer <= 0.0f)
    {
        sound_play(g_asset_sfx_alarm_clock, 0);
    }
    if(prev_timer > 5.0f && g_state.timer <= 5.0f)
    {
        sound_play(g_asset_sfx_clock_ticking, 0);
    }
}

static void game_render_timer(void)
{
    static const nkF32 LETTER_WIDTH = 15.0f;
    static const nkF32 PADDING = 4.0f;
    static const nkF32 DANGER_TIME = 5.0f;

    nkF32 width = LETTER_WIDTH * 5.0f;

    imm_begin_texture_batch(g_asset_ui);

    nkF32 x = (SCREEN_WIDTH - width) * 0.5f;
    nkF32 y = PADDING;

    nkF32 ox = (g_state.timer <= 0.0f) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;
    nkF32 oy = (g_state.timer <= 0.0f) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;

    x -= (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.w * 0.5f) + PADDING;
    y += (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.h * 0.5f);

    render_item(x+ox,y+oy, ATLAS_UI, ATLAS_UI_CLOCK_BODY, 1.0f);

    nkChar timer_buffer[8] = NK_ZERO_MEM;
    sprintf(timer_buffer, "%04.1f0", g_state.timer);

    x += (ATLAS_UI[ATLAS_UI_CLOCK_BODY].clip_bounds.w * 0.5f) + PADDING;

    for(nkU32 i=0,n=strlen(timer_buffer); i<n; ++i)
    {
        ox = (g_state.timer <= DANGER_TIME) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;
        oy = (g_state.timer <= DANGER_TIME) ? rng_int_range(&g_rng_v, -1,1) : 0.0f;

        nkS32 index = ATLAS_UI_TIMER_0_SHADOW + (((timer_buffer[i] - '0') * 2) + 1);
        if(timer_buffer[i] == '.') index = ATLAS_UI_TIMER_DOT_BODY;

        x += LETTER_WIDTH * 0.5f;
        render_item_ex(x+ox,y+oy, 0.7f,0.7f, 0.0f, ATLAS_UI, index, 1.0f);
        x += LETTER_WIDTH * 0.5f;
    }

    imm_end_texture_batch();
}

static void game_render(void)
{
    nkF32 hsw = SCREEN_WIDTH * 0.5f;
    nkF32 hsh = SCREEN_HEIGHT * 0.5f;

    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    imm_texture(g_asset_background, hsw,hsh, NULL, NK_V4_WHITE);

    switch(g_state.minigame)
    {
        case MiniGame_Typer: minigame_typer_render(); break;
    }

    game_render_timer();
}

static nkBool game_is_playing(void)
{
    return (g_state.timer > 0.0f);
}

static nkF32 game_get_timer(void)
{
    return g_state.timer;
}

/*////////////////////////////////////////////////////////////////////////////*/
