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

static nkBool update_text_button(const nkChar* text, nkF32 y, nkF32 scale)
{
    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    return (cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h) && is_mouse_button_pressed(MouseButton_Left));
}

static void render_text_button(const nkChar* text, nkF32 y, nkF32 scale)
{
    FontStyle style = FontStyle_Faded;
    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    if(cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h))
        style = FontStyle_Rotate;
    render_bitmap_font_aligned(text, Alignment_Center, y, scale, style);
}


static void app_init(void)
{
    rng_init(time(NULL));

    load_all_assets();

    show_cursor(NK_FALSE);

    menu_init();
    game_init();

    g_appstate = AppState_Menu;

    // Setup the initial angles.
    for(nkS32 i=0; i<APP_MAX_ANGLES; ++i)
    {
        g_angles_big[i] = rng_num_range(-0.4f,0.4f);
        g_angles_lil[i] = rng_num_range(-0.1f,0.1f);
    }
}

static void app_quit(void)
{
    game_quit();
    menu_quit();

    free_all_assets();
}

static nkF32 app_update_angle(nkF32 old_angle, nkF32 min, nkF32 max)
{
    nkF32 min_delta = 0.15f * (fabsf(min / 0.4f));
    nkF32 max_delta = 0.25f * (fabsf(max / 0.4f));
    nkF32 new_angle = old_angle;
    while(fabsf(old_angle-new_angle) <= min_delta || fabsf(old_angle-new_angle) >= max_delta)
        new_angle = rng_num_range(min,max);
    return new_angle;
}
static void app_update(nkF32 dt)
{
    // Update the render angles at a fixed interval.
    g_angle_timer += dt;
    if(g_angle_timer >= APP_ANGLE_CHANGE_SPEED)
    {
        g_angle_timer -= APP_ANGLE_CHANGE_SPEED;
        for(nkS32 i=0; i<APP_MAX_ANGLES; ++i)
        {
            g_angles_big[i] = app_update_angle(g_angles_big[i], -0.4f,0.4f);
            g_angles_lil[i] = app_update_angle(g_angles_lil[i], -0.1f,0.1f);
        }
    }

    cursor_update(dt);

    if(!is_game_paused())
    {
        switch(g_appstate)
        {
            case AppState_Menu: menu_update(dt); break;
            case AppState_Game: game_update(dt); break;
        }
    }

    // Make sure to go after normal update stuff to avoid issues...
    pause_update(dt);
}

static void app_render(void)
{
    nkF32 hsw = SCREEN_WIDTH * 0.5f;
    nkF32 hsh = SCREEN_HEIGHT * 0.5f;

    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    #if 1
    imm_texture(g_asset_back_paper, hsw,hsh, NULL, NK_V4_WHITE);
    #else
    imm_texture(g_asset_back_vignette, hsw,hsh, NULL, NK_V4_WHITE);
    #endif

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
