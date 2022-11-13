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

static void game_init(void)
{
    g_state.minigame = MiniGame_Typer;
    load_all_assets();
    show_cursor(NK_FALSE);

    g_state.timer = 20.0f;

    minigame_typer_init();
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

    g_state.timer -= dt;
    if(g_state.timer < 0.0f)
    {
        g_state.timer = 0.0f;
    }
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

    // Draw the game timer. @Temporary: Just using a debug font for now...
    nkChar timer_buffer[32] = NK_ZERO_MEM;
    sprintf(timer_buffer, "%05.2f", g_state.timer);

    nkF32 tw = font_get_text_width(g_asset_font, "20.00");

    nkF32 tx = (SCREEN_WIDTH - tw) * 0.5f;
    nkF32 ty = font_get_px_height(g_asset_font) * 0.75f;

    nkVec4 fg_color = { 0.15f,0.12f,0.10f,1.0f };
    nkVec4 bg_color = { 0.00f,0.00f,0.00f,0.3f };

    font_draw_text(g_asset_font, tx+2,ty+2, timer_buffer, bg_color);
    font_draw_text(g_asset_font, tx,ty, timer_buffer, fg_color);
}

/*////////////////////////////////////////////////////////////////////////////*/
