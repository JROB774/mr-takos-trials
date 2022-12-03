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

static nkF32 render_score(nkF32 x, nkF32 y, nkS32 score)
{
    static const nkF32 LETTER_WIDTH = 15.0f;

    nkChar score_buffer[16] = NK_ZERO_MEM;
    sprintf(score_buffer, "%05d", score);

    x -= ((LETTER_WIDTH*strlen(score_buffer)) * 0.5f);

    nkF32 new_x = x;

    for(nkU32 i=0,n=strlen(score_buffer); i<n; ++i)
    {
        nkS32 index = ATLAS_UI_TIMER_0_SHADOW + (((score_buffer[i] - '0') * 2) + 1);
        x += LETTER_WIDTH * 0.5f;
        render_item_ex(x,y, 0.7f,0.7f, 0.0f, ATLAS_UI, index, 0.7f);
        x += LETTER_WIDTH * 0.5f;
    }

    return new_x;
}

static void change_page(void)
{
    nkS32 old_background = g_background;
    while(old_background == g_background)
        g_background = rng_int() % APP_MAX_BACKGROUNDS;
    sound_play(g_asset_sfx_page_flip[rng_int_range(0,10)], 0);
}

static void app_init(void)
{
    rng_init(time(NULL));

    load_all_assets();

    show_cursor(NK_FALSE);

    menu_init();
    game_init();

    g_appstate = AppState_Menu;

    g_background = rng_int() % APP_MAX_BACKGROUNDS;

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
    particles_update(dt);

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

    ImmRect clip = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };
    clip.y = SCREEN_HEIGHT * g_background;
    imm_texture(g_asset_background, hsw,hsh, &clip, NK_V4_WHITE);

    if(!is_game_paused())
    {
        switch(g_appstate)
        {
            case AppState_Menu: menu_render(); break;
            case AppState_Game: game_render(); break;
        }
    }

    particles_render();

    pause_render();
    cursor_render();
}

/*////////////////////////////////////////////////////////////////////////////*/
