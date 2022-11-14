/*////////////////////////////////////////////////////////////////////////////*/

NK_ENUM(MainMenuOption, nkS32)
{
    MainMenuOption_Play,
    MainMenuOption_Options,
    MainMenuOption_Awards,
    MainMenuOption_Credits,
    MainMenuOption_Exit,
    MainMenuOption_TOTAL
};

static const nkChar* MAIN_MENU_OPTIONS[] =
{
    "Play Game",
    "Options",
    "Awards",
    "Credits",
    "Exit"
};

NK_STATIC_ASSERT(MainMenuOption_TOTAL == NK_ARRAY_SIZE(MAIN_MENU_OPTIONS), main_menu_option_size_mismatch);

static void menu_init(void)
{
    // Nothing...
}

static void menu_quit(void)
{
    // Nothing...
}

static void menu_update(nkF32 dt)
{
    nkF32 h = font_get_px_height(g_asset_font) * 0.75f;
    nkF32 y = ((SCREEN_HEIGHT - ((MainMenuOption_TOTAL-1) * h)) * 0.5f) + (h * 0.25f);

    for(nkS32 i=0; i<MainMenuOption_TOTAL; ++i)
    {
        nkF32 w = font_get_text_bounds(g_asset_font, MAIN_MENU_OPTIONS[i]).x;
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

static void menu_render(void)
{
    nkF32 h = font_get_px_height(g_asset_font) * 0.75f;
    nkF32 y = ((SCREEN_HEIGHT - ((MainMenuOption_TOTAL-1) * h)) * 0.5f) + (h * 0.25f);

    for(nkS32 i=0; i<MainMenuOption_TOTAL; ++i)
    {
        nkF32 w = font_get_text_bounds(g_asset_font, MAIN_MENU_OPTIONS[i]).x;
        nkF32 x = (SCREEN_WIDTH - w) * 0.5f;

        nkVec4 fg_color = (nkVec4){ 0.15f,0.10f,0.00f,1.0f };
        nkVec4 bg_color = (nkVec4){ 0.00f,0.00f,0.00f,0.3f };

        if(cursor_in_bounds(x,y-(h*0.75f),w,h))
        {
            fg_color.r += 0.3f;
            fg_color.g += 0.3f;
            fg_color.b += 0.3f;
        }

        font_draw_text(g_asset_font, x+2,y+2, MAIN_MENU_OPTIONS[i], bg_color);
        font_draw_text(g_asset_font, x,y, MAIN_MENU_OPTIONS[i], fg_color);

        y += h;
    }
}

/*////////////////////////////////////////////////////////////////////////////*/
