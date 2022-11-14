/*////////////////////////////////////////////////////////////////////////////*/

static void render_item(nkF32 x, nkF32 y, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 shadow_offset_multiplier)
{
    nkF32 sx = x + (SHADOW_OFFSET.x * shadow_offset_multiplier);
    nkF32 sy = y + (SHADOW_OFFSET.y * shadow_offset_multiplier);

    imm_atlas_batched(sx,sy, &atlas_clips[atlas_clip_index-1], SHADOW_COLOR);
    imm_atlas_batched(x,y, &atlas_clips[atlas_clip_index], BODY_COLOR);
}

static void render_item_ex(nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 shadow_offset_multiplier)
{
    nkF32 xs = x + (SHADOW_OFFSET.x * shadow_offset_multiplier);
    nkF32 ys = y + (SHADOW_OFFSET.y * shadow_offset_multiplier);

    imm_atlas_batched_ex(xs,ys, sx,sy, angle, NULL, &atlas_clips[atlas_clip_index-1], SHADOW_COLOR);
    imm_atlas_batched_ex(x,y, sx,sy, angle, NULL, &atlas_clips[atlas_clip_index], BODY_COLOR);
}

static void app_init(void)
{
    nkU32 seed = time(NULL);

    rng_init(&g_rng_l, seed);
    rng_init(&g_rng_v, seed);

    load_all_assets();

    show_cursor(NK_FALSE);

    menu_init();
    game_init();

    g_appstate = AppState_Menu;
}

static void app_quit(void)
{
    game_quit();
    menu_quit();

    free_all_assets();
}

static void app_update(nkF32 dt)
{
    pause_update(dt);
    cursor_update(dt);

    if(!is_game_paused())
    {
        switch(g_appstate)
        {
            case AppState_Menu: menu_update(dt); break;
            case AppState_Game: game_update(dt); break;
        }
    }
}

static void app_render(void)
{
    nkF32 hsw = SCREEN_WIDTH * 0.5f;
    nkF32 hsh = SCREEN_HEIGHT * 0.5f;

    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    imm_texture(g_asset_background, hsw,hsh, NULL, NK_V4_WHITE);

    if(!is_game_paused())
    {
        switch(g_appstate)
        {
            case AppState_Menu: menu_render(); break;
            case AppState_Game: game_render(); break;
        }
    }

    pause_render();
    cursor_render();
}

/*////////////////////////////////////////////////////////////////////////////*/
