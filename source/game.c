/*////////////////////////////////////////////////////////////////////////////*/

NK_ENUM(MiniGame, nkS32)
{
    MiniGame_Typer,
    MiniGame_TOTAL
};

static MiniGame g_minigame;

static void game_init(void)
{
    g_minigame = MiniGame_Typer;
    load_all_assets();
    show_cursor(NK_FALSE);

    minigame_typer_init();
}

static void game_quit(void)
{
    minigame_typer_quit();

    free_all_assets();
}

static void game_update(nkF32 dt)
{
    switch(g_minigame)
    {
        case MiniGame_Typer: minigame_typer_update(dt); break;
    }
}

static void game_render(void)
{
    nkF32 hsw = SCREEN_WIDTH * 0.5f;
    nkF32 hsh = SCREEN_HEIGHT * 0.5f;

    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    imm_texture(g_asset_background, hsw,hsh, NULL, NK_V4_WHITE);

    switch(g_minigame)
    {
        case MiniGame_Typer: minigame_typer_render(); break;
    }
}

/*////////////////////////////////////////////////////////////////////////////*/
