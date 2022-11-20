/*////////////////////////////////////////////////////////////////////////////*/

#define UI_SLIDER_GAP 12.0f

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

static nkBool update_simple_button(const nkChar* text, nkF32 y, nkF32 scale)
{
    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    return (cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h) && is_mouse_button_pressed(MouseButton_Left));
}

static void render_simple_button(const nkChar* text, nkF32 y, nkF32 scale)
{
    FontStyle style = FontStyle_Faded;
    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    if(cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h))
        style = FontStyle_Rotate;
    render_bitmap_font_aligned(text, Alignment_Center, y, scale, style);
}

static nkBool update_toggle_button(const nkChar* text_a, const nkChar* text_b, nkBool toggle, nkF32 y, nkF32 scale)
{
    return update_simple_button((toggle) ? text_a : text_b, y, scale);
}

static void render_toggle_button(const nkChar* text_a, const nkChar* text_b, nkBool toggle, nkF32 y, nkF32 scale)
{
    render_simple_button((toggle) ? text_a : text_b, y, scale);
}

static nkF32 update_slider_button(const nkChar* text, nkF32 value, nkF32 y, nkF32 scale)
{
    nkF32 segment_width = ATLAS_UI[ATLAS_UI_SLIDER_FADED_EMPTY_BODY].clip_bounds.w * scale;

    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    bounds.x -= (((segment_width * 10) + UI_SLIDER_GAP) * 0.5f);
    nkF32 x = bounds.x + bounds.w + UI_SLIDER_GAP; // The start of the slider segments.
    bounds.w += (((segment_width * 10) + UI_SLIDER_GAP));

    nkBool hovered = cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h);
    ImmRect c = cursor_get_bounds();
    if(hovered && is_mouse_button_pressed(MouseButton_Left))
    {
        if((c.x < x))
        {
            value = 0.0f;
        }
        else
        {
            for(nkS32 i=0; i<10; ++i)
            {
                if((c.x > x) && is_mouse_button_pressed(MouseButton_Left))
                    value = NK_CAST(nkF32, (i+1)) / 10.0f;
                x += segment_width;
            }
        }
    }

    return value;
}

static void render_slider_button(const nkChar* text, nkF32 value, nkF32 y, nkF32 scale)
{
    nkF32 segment_width = ATLAS_UI[ATLAS_UI_SLIDER_FADED_EMPTY_BODY].clip_bounds.w * scale;
    nkS32 segments_filled = NK_CAST(nkS32, value * 10.0f);

    nkS32 text_length = strlen(text);

    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    bounds.x -= (((segment_width * 10) + UI_SLIDER_GAP) * 0.5f);
    nkF32 x = bounds.x + bounds.w + UI_SLIDER_GAP; // The start of the slider segments.
    bounds.w += (((segment_width * 10) + UI_SLIDER_GAP));

    nkBool hovered = cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h);

    FontStyle style = (hovered) ? FontStyle_Rotate : FontStyle_Faded;
    render_bitmap_font(text, bounds.x, y, scale, style);

    imm_begin_texture_batch(g_asset_ui);
    for(nkS32 i=0; i<10; ++i)
    {
        nkS32 index = ATLAS_UI_SLIDER_FADED_EMPTY_BODY;
        if(!NK_CHECK_FLAGS(style, FontStyle_Faded))
            index = ATLAS_UI_SLIDER_SOLID_EMPTY_BODY;
        if(hovered)
        {
            // Show the new value based on the cursor position.
            ImmRect c = cursor_get_bounds();
            if(c.x >= x) index += 2;
        }
        else
        {
            if(segments_filled > i) index += 2;
        }

        nkF32 angle = 0.0f;
        if(NK_CHECK_FLAGS(style, FontStyle_Rotate))
        {
            angle = g_angles_lil[i+text_length];
        }

        x += (segment_width * 0.5f);
        render_item_ex(x,y, scale,scale, angle, ATLAS_UI, index, 1.0f);
        x += (segment_width * 0.5f);
    }
    imm_end_texture_batch();
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
