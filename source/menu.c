/*////////////////////////////////////////////////////////////////////////////*/

NK_ENUM(MenuState, nkS32)
{
    MenuState_Title,
    MenuState_Main,
    MenuState_Options,
    MenuState_Awards,
    MenuState_Credits,
    MenuState_TOTAL
};

NK_ENUM(MainMenuOption, nkS32)
{
    MainMenuOption_Play,
    MainMenuOption_Options,
    MainMenuOption_Awards,
    MainMenuOption_Credits,
    MainMenuOption_Exit,
    MainMenuOption_TOTAL
};

static const nkChar* MAIN_MENU_OPTIONS[] = { "Play Game", "Options", "Awards", "Credits", "Exit" };

NK_STATIC_ASSERT(MainMenuOption_TOTAL == NK_ARRAY_SIZE(MAIN_MENU_OPTIONS), main_menu_option_size_mismatch);

static MenuState g_menustate;

static void menu_init(void)
{
    g_menustate = MenuState_Title;
}

static void menu_quit(void)
{
    // Nothing...
}

static void menu_update_title(nkF32 dt)
{
    if(is_mouse_button_pressed(MouseButton_Left))
    {
        g_menustate = MenuState_Main;
    }
}

static void menu_update_main(nkF32 dt)
{
    nkF32 h = font_get_px_height(g_asset_font_big) * 0.75f;
    nkF32 y = ((SCREEN_HEIGHT - ((MainMenuOption_TOTAL-1) * h)) * 0.5f) + (h * 0.25f);

    for(nkS32 i=0; i<MainMenuOption_TOTAL; ++i)
    {
        nkF32 w = font_get_text_bounds(g_asset_font_big, MAIN_MENU_OPTIONS[i]).x;
        nkF32 x = (SCREEN_WIDTH - w) * 0.5f;

        if(cursor_in_bounds(x,y-(h*0.75f),w,h) && is_mouse_button_pressed(MouseButton_Left))
        {
            switch(i)
            {
                case MainMenuOption_Play:
                {
                    g_appstate = AppState_Game;
                    game_start();
                } break;
                case MainMenuOption_Options: /* @Incomplete */ break;
                case MainMenuOption_Awards: /* @Incomplete */ break;
                case MainMenuOption_Credits: /* @Incomplete */ break;
                case MainMenuOption_Exit: terminate(); break;
            }
        }

        y += h;
    }
}

static void menu_update_options(nkF32 dt)
{
    // @Incomplete: ...
}

static void menu_update_awards(nkF32 dt)
{
    // @Incomplete: ...
}

static void menu_update_credits(nkF32 dt)
{
    // @Incomplete: ...
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

static void menu_render_title(void)
{
    // @Incomplete: ...
}

static void menu_render_main(void)
{
    nkF32 h = font_get_px_height(g_asset_font_big) * 0.75f;
    nkF32 y = ((SCREEN_HEIGHT - ((MainMenuOption_TOTAL-1) * h)) * 0.5f) + (h * 0.25f);

    for(nkS32 i=0; i<MainMenuOption_TOTAL; ++i)
    {
        nkF32 w = font_get_text_bounds(g_asset_font_big, MAIN_MENU_OPTIONS[i]).x;
        nkF32 x = (SCREEN_WIDTH - w) * 0.5f;

        nkVec4 fg_color = (nkVec4){ 0.15f,0.10f,0.00f,1.0f };
        nkVec4 bg_color = (nkVec4){ 0.00f,0.00f,0.00f,0.3f };

        if(cursor_in_bounds(x,y-(h*0.5f),w,(h*0.5f)))
        {
            fg_color.r += 0.3f;
            fg_color.g += 0.3f;
            fg_color.b += 0.3f;
        }

        font_draw_text(g_asset_font_big, x+2,y+2, MAIN_MENU_OPTIONS[i], bg_color);
        font_draw_text(g_asset_font_big, x,y, MAIN_MENU_OPTIONS[i], fg_color);

        y += h;
    }
}

static void menu_render_options(void)
{
    // @Incomplete: ...
}

static void menu_render_awards(void)
{
    // @Incomplete: ...
}

static void menu_render_credits(void)
{
    // @Incomplete: ...
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
