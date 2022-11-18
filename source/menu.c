/*////////////////////////////////////////////////////////////////////////////*/

#define BACK_BUTTON_X  0.0f
#define BACK_BUTTON_Y  0.0f
#define BACK_BUTTON_W 60.0f
#define BACK_BUTTON_H 60.0f

#define MENU_TEXT_SCALE 0.7f

NK_ENUM(MenuState, nkS32)
{
    MenuState_Title,
    MenuState_Main,
    MenuState_Options,
    MenuState_Awards,
    MenuState_Credits,
    MenuState_TOTAL
};

static MenuState g_menustate;

static void change_menu_state(MenuState new_state)
{
    sound_play(g_asset_sfx_page_flip[rng_int_range(0,10)], 0);
    g_menustate = new_state;
}

static void update_back_button(void)
{
    nkF32 x = BACK_BUTTON_X;
    nkF32 y = BACK_BUTTON_Y;
    nkF32 w = BACK_BUTTON_W;
    nkF32 h = BACK_BUTTON_H;

    if(cursor_in_bounds(x,y,w,h) && is_mouse_button_pressed(MouseButton_Left))
    {
        change_menu_state(MenuState_Main);
    }
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
    // @Incomplete: Debug title (not final visuals).
    // Render the title.
    nkF32 h = font_get_px_height(g_asset_font_big) * 0.75f;
    nkF32 y = ((SCREEN_HEIGHT - (4 * h)) * 0.5f) + (h * 0.25f);
    nkF32 w = font_get_text_bounds(g_asset_font_big, "Mr. Tako's Trials").x;
    nkF32 x = (SCREEN_WIDTH - w) * 0.5f;
    font_draw_text(g_asset_font_big, x+2,y+2, "Mr. Tako's Trials", DEBUG_FONT_BG_COLOR);
    font_draw_text(g_asset_font_big, x,y, "Mr. Tako's Trials", DEBUG_FONT_FG_COLOR);

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
        if(update_text_button(MAIN_MENU_OPTIONS[i], y, MENU_TEXT_SCALE))
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
        render_text_button(MAIN_MENU_OPTIONS[i], y, MENU_TEXT_SCALE);
        y += bitmap_font_line_advance(MENU_TEXT_SCALE);
    }
}

// =============================================================================
// Options
// =============================================================================

static void menu_update_options(nkF32 dt)
{
    if(is_key_pressed(KeyCode_Escape))
        change_menu_state(MenuState_Main);
    update_back_button();

    // @Incomplete: ...
}

static void menu_render_options(void)
{
    render_back_button();
    // @Incomplete: ...
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
    switch(g_menustate)
    {
        case MenuState_Title: menu_update_title(dt); break;
        case MenuState_Main: menu_update_main(dt); break;
        case MenuState_Options: menu_update_options(dt); break;
        case MenuState_Awards: menu_update_awards(dt); break;
        case MenuState_Credits: menu_update_credits(dt); break;
    }
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
