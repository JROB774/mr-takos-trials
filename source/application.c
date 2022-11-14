/*////////////////////////////////////////////////////////////////////////////*/

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
    switch(g_appstate)
    {
        case AppState_Menu: menu_update(dt); break;
        case AppState_Game: game_update(dt); break;
    }
}

static void app_render(void)
{
    nkF32 hsw = SCREEN_WIDTH * 0.5f;
    nkF32 hsh = SCREEN_HEIGHT * 0.5f;

    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    imm_texture(g_asset_background, hsw,hsh, NULL, NK_V4_WHITE);

    switch(g_appstate)
    {
        case AppState_Menu: menu_render(); break;
        case AppState_Game: game_render(); break;
    }

    // @Incomplete: Hide the mouse cursor when it has been inactive long enough...
    nkVec2 mp = get_screen_mouse_pos();
    imm_begin_texture_batch(g_asset_ui);
    render_item(mp.x,mp.y, ATLAS_UI, ATLAS_UI_CURSOR_BODY, 1.0f);
    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
