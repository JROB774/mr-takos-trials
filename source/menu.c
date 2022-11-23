/*////////////////////////////////////////////////////////////////////////////*/

#define BACK_BUTTON_X  0.0f
#define BACK_BUTTON_Y  0.0f
#define BACK_BUTTON_W 60.0f
#define BACK_BUTTON_H 60.0f

#define MENU_TEXT_SCALE 0.7f
#define UI_SLIDER_GAP  12.0f

NK_ENUM(MenuState, nkS32)
{
    MenuState_Title,
    MenuState_Main,
    MenuState_Options,
    MenuState_Awards,
    MenuState_Credits,
    MenuState_TOTAL
};

static MenuState     g_menustate;
static const nkChar* g_menu_curr_hovered;
static const nkChar* g_menu_prev_hovered;
static nkU32         g_resetsave;

static void change_menu_state(MenuState new_state)
{
    sound_play(g_asset_sfx_page_flip[rng_int_range(0,10)], 0);
    g_menustate = new_state;
    g_resetsave = 0; // Revert the "reset save" button clicks value.
}

static nkBool update_back_button(void)
{
    nkF32 x = BACK_BUTTON_X;
    nkF32 y = BACK_BUTTON_Y;
    nkF32 w = BACK_BUTTON_W;
    nkF32 h = BACK_BUTTON_H;

    if(cursor_in_bounds(x,y,w,h) && is_mouse_button_pressed(MouseButton_Left))
    {
        change_menu_state(MenuState_Main);
        return NK_TRUE;
    }

    return NK_FALSE;
}

static void render_back_button(void)
{
    nkF32 x = BACK_BUTTON_X;
    nkF32 y = BACK_BUTTON_Y;
    nkF32 w = BACK_BUTTON_W;
    nkF32 h = BACK_BUTTON_H;

    nkF32 bx = x + (w * 0.5f);
    nkF32 by = y + (h * 0.5f);

    imm_begin_texture_batch(g_asset_ui);
    if(cursor_in_bounds(x,y,w,h))
    {
        nkF32 scale = (is_mouse_button_pressed(MouseButton_Left)) ? 1.3f : 1.1f;
        render_item_ex(bx,by, scale,scale, g_angles_big[APP_MAX_ANGLES-3], ATLAS_UI, ATLAS_UI_BACK_BODY, 1.0f);
    }
    else
    {
        render_item(bx,by, ATLAS_UI, ATLAS_UI_BACK_BODY, 1.0f);
    }
    imm_end_texture_batch();
}

static nkBool update_simple_button(const nkChar* text, nkF32 y, nkF32 scale)
{
    ImmRect bounds = get_bitmap_font_bounds_aligned(text, Alignment_Center, y, scale, FontStyle_None);
    nkBool hovered = cursor_in_bounds(bounds.x,bounds.y,bounds.w,bounds.h);
    if(hovered) g_menu_curr_hovered = text;
    return (hovered && is_mouse_button_pressed(MouseButton_Left));
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
    if(hovered)
    {
        g_menu_curr_hovered = text;
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

// =============================================================================
// Title
// =============================================================================

static void menu_update_title(nkF32 dt)
{
    if(is_mouse_button_pressed(MouseButton_Left))
    {
        change_menu_state(MenuState_Main);
    }
}

static void menu_render_title(void)
{
    // Render the title.
    nkF32 h = font_get_px_height(g_asset_debug_font_big) * 0.75f;
    nkF32 y = ((SCREEN_HEIGHT - (4 * h)) * 0.5f) + (h * 0.25f);
    nkF32 w = font_get_text_bounds(g_asset_debug_font_big, "Mr. Tako's Trials").x;
    nkF32 x = (SCREEN_WIDTH - w) * 0.5f;
    font_draw_text(g_asset_debug_font_big, x+2,y+2, "Mr. Tako's Trials", DEBUG_FONT_BG_COLOR);
    font_draw_text(g_asset_debug_font_big, x,y, "Mr. Tako's Trials", DEBUG_FONT_FG_COLOR);

    // Render the "click to start" prompt.
    imm_begin_texture_batch(g_asset_ui);
    render_item(SCREEN_WIDTH * 0.5f, 220.0f, ATLAS_UI, ATLAS_UI_START_BODY, 1.0f);
    imm_end_texture_batch();
}

// =============================================================================
// Main
// =============================================================================

NK_ENUM(MainMenuOption, nkS32)
{
    MainMenuOption_Play,
    MainMenuOption_Options,
    MainMenuOption_Awards,
    MainMenuOption_Credits,
    MainMenuOption_Exit,
    MainMenuOption_TOTAL
};

static const nkChar* MAIN_MENU_OPTIONS[] = { "PLAY", "OPTIONS", "AWARDS", "CREDITS", "EXIT" };

NK_STATIC_ASSERT(MainMenuOption_TOTAL == NK_ARRAY_SIZE(MAIN_MENU_OPTIONS), main_menu_option_size_mismatch);

static void menu_update_main(nkF32 dt)
{
    if(is_key_pressed(KeyCode_Escape))
    {
        change_menu_state(MenuState_Title);
    }

    nkF32 y = bitmap_font_block_y_off(MainMenuOption_TOTAL, MENU_TEXT_SCALE);
    for(nkS32 i=0; i<MainMenuOption_TOTAL; ++i)
    {
        if(update_simple_button(MAIN_MENU_OPTIONS[i], y, MENU_TEXT_SCALE))
        {
            switch(i)
            {
                case MainMenuOption_Play:
                {
                    g_appstate = AppState_Game;
                    game_start();
                } break;

                case MainMenuOption_Options: change_menu_state(MenuState_Options); break;
                case MainMenuOption_Awards: change_menu_state(MenuState_Awards); break;
                case MainMenuOption_Credits: change_menu_state(MenuState_Credits); break;

                case MainMenuOption_Exit: terminate(); break;
            }
        }
        y += bitmap_font_line_advance(MENU_TEXT_SCALE);
    }
}

static void menu_render_main(void)
{
    nkF32 y = bitmap_font_block_y_off(MainMenuOption_TOTAL, MENU_TEXT_SCALE);
    for(nkS32 i=0; i<MainMenuOption_TOTAL; ++i)
    {
        render_simple_button(MAIN_MENU_OPTIONS[i], y, MENU_TEXT_SCALE);
        y += bitmap_font_line_advance(MENU_TEXT_SCALE);
    }
}

// =============================================================================
// Options
// =============================================================================

static const nkChar* RESET_SAVE_LABELS[4] = { "RESET SAVE", "ARE YOU SURE", "LAST CHANCE", "SAVE NUKED" };
static Sound*        RESET_SAVE_SOUNDS[3] = { &g_asset_sfx_gasp_small, &g_asset_sfx_gasp_large, &g_asset_sfx_explosion };

NK_ENUM(OptionsMenuOption, nkS32)
{
    OptionsMenuOption_Window,
    OptionsMenuOption_Sound,
    OptionsMenuOption_Music,
    OptionsMenuOption_Reset,
    OptionsMenuOption_TOTAL
};

static void menu_update_options(nkF32 dt)
{
    if(is_key_pressed(KeyCode_Escape))
        change_menu_state(MenuState_Main);
    update_back_button();

    nkF32 y = bitmap_font_block_y_off(OptionsMenuOption_TOTAL, MENU_TEXT_SCALE);

    // Fullscreen toggle.
    if(update_toggle_button("WINDOWED", "FULLSCREEN", get_fullscreen(), y, MENU_TEXT_SCALE))
        set_fullscreen(!get_fullscreen());
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);

    // Sound slider.
    nkF32 sound = update_slider_button("SOUND", get_sound_volume(), y, MENU_TEXT_SCALE);
    if(sound != get_sound_volume()) set_sound_volume(sound);
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);

    // Music slider.
    nkF32 music = update_slider_button("MUSIC", get_music_volume(), y, MENU_TEXT_SCALE);
    if(music != get_music_volume()) set_music_volume(music);
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);

    // Reset save stages.
    if(update_simple_button(RESET_SAVE_LABELS[g_resetsave], y, MENU_TEXT_SCALE) && g_resetsave < 3)
    {
        sound_play(*RESET_SAVE_SOUNDS[g_resetsave], 0);
        g_resetsave++;
        if(g_resetsave == 3)
        {
            sound_play(g_asset_sfx_cat_meow, 0);
            reset_game_data();
        }
    }
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);
}

static void menu_render_options(void)
{
    render_back_button();

    nkF32 y = bitmap_font_block_y_off(OptionsMenuOption_TOTAL, MENU_TEXT_SCALE);

    // Fullscreen toggle.
    render_toggle_button("WINDOWED", "FULLSCREEN", get_fullscreen(), y, MENU_TEXT_SCALE);
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);

    // Sound slider.
    render_slider_button("SOUND", get_sound_volume(), y, MENU_TEXT_SCALE);
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);

    // Music slider.
    render_slider_button("MUSIC", get_music_volume(), y, MENU_TEXT_SCALE);
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);

    // Reset save stages.
    render_simple_button(RESET_SAVE_LABELS[g_resetsave], y, MENU_TEXT_SCALE);
    y += bitmap_font_line_advance(MENU_TEXT_SCALE);
}

// =============================================================================
// Awards
// =============================================================================

static void menu_update_awards(nkF32 dt)
{
    if(is_key_pressed(KeyCode_Escape))
        change_menu_state(MenuState_Main);
    update_back_button();

    // @Incomplete: ...
}

static void menu_render_awards(void)
{
    render_back_button();
    // @Incomplete: ...
}

// =============================================================================
// Credits
// =============================================================================

static void menu_update_credits(nkF32 dt)
{
    if(is_key_pressed(KeyCode_Escape))
        change_menu_state(MenuState_Main);
    update_back_button();

    // @Incomplete: ...
}

static void menu_render_credits(void)
{
    render_back_button();
    // @Incomplete: ...
}

// =============================================================================
// Menu System
// =============================================================================

static void menu_init(void)
{
    g_menustate = MenuState_Title;
}

static void menu_quit(void)
{
    // Nothing...
}

static void menu_update(nkF32 dt)
{
    g_menu_curr_hovered = NULL;

    switch(g_menustate)
    {
        case MenuState_Title: menu_update_title(dt); break;
        case MenuState_Main: menu_update_main(dt); break;
        case MenuState_Options: menu_update_options(dt); break;
        case MenuState_Awards: menu_update_awards(dt); break;
        case MenuState_Credits: menu_update_credits(dt); break;
    }

    // NOTE: Doesn't sound the best, if we can get a better sound then maybe we should do this...
    //
    // if(g_menu_curr_hovered && (g_menu_curr_hovered != g_menu_prev_hovered))
    //     sound_play(g_asset_sfx_paper_rustle[rng_int_range(0,1)], 0);

    g_menu_prev_hovered = g_menu_curr_hovered;
}

static void menu_render(void)
{
    switch(g_menustate)
    {
        case MenuState_Title: menu_render_title(); break;
        case MenuState_Main: menu_render_main(); break;
        case MenuState_Options: menu_render_options(); break;
        case MenuState_Awards: menu_render_awards(); break;
        case MenuState_Credits: menu_render_credits(); break;
    }
}

/*////////////////////////////////////////////////////////////////////////////*/
