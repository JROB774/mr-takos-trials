/*////////////////////////////////////////////////////////////////////////////*/

#define PAUSE_BUTTON_X (SCREEN_WIDTH - 60.0f)
#define PAUSE_BUTTON_Y (                0.0f)
#define PAUSE_BUTTON_W (               60.0f)
#define PAUSE_BUTTON_H (               60.0f)

typedef struct PauseState
{
    nkBool paused;
}
PauseState;

static PauseState g_pause;

static void pause_update(nkF32 dt)
{
    // Cannot be paused in any other state but the game.
    if(g_appstate != AppState_Game)
    {
        g_pause.paused = NK_FALSE;
        return;
    }

    // Check for the pause button hotkey.
    if(is_key_pressed(KeyCode_Escape))
    {
        g_pause.paused = !g_pause.paused;
    }

    // Do the pause button or menu logic.
    if(g_pause.paused)
    {
        // Update pause menu.
        // @Incomplete: ...
    }
    else
    {
        nkF32 x = PAUSE_BUTTON_X;
        nkF32 y = PAUSE_BUTTON_Y;
        nkF32 w = PAUSE_BUTTON_W;
        nkF32 h = PAUSE_BUTTON_H;

        if(cursor_in_bounds(x,y,w,h) && is_mouse_button_pressed(MouseButton_Left))
        {
            g_pause.paused = NK_TRUE;
        }
    }
}

static void pause_render(void)
{
    // Cannot be paused in any other state but the game.
    if(g_appstate != AppState_Game)
    {
        g_pause.paused = NK_FALSE;
        return;
    }

    // Do the pause button or menu logic.
    if(g_pause.paused)
    {
        // Render pause menu.
        nkF32 w = font_get_text_bounds(g_asset_font_big, "Paused").x;
        nkF32 h = font_get_px_height(g_asset_font_big) * 0.75f;
        nkF32 x = (SCREEN_WIDTH - w) * 0.5f;
        nkF32 y = (SCREEN_HEIGHT * 0.5f) + (h * 0.25f);

        nkVec4 fg_color = (nkVec4){ 0.15f,0.10f,0.00f,1.0f };
        nkVec4 bg_color = (nkVec4){ 0.00f,0.00f,0.00f,0.3f };

        font_draw_text(g_asset_font_big, x+2,y+2, "Paused", bg_color);
        font_draw_text(g_asset_font_big, x,y, "Paused", fg_color);
    }
    else
    {
        // Render the pause button.
        nkF32 x = PAUSE_BUTTON_X;
        nkF32 y = PAUSE_BUTTON_Y;
        nkF32 w = PAUSE_BUTTON_W;
        nkF32 h = PAUSE_BUTTON_H;

        nkF32 px = x + (w * 0.5f);
        nkF32 py = y + (h * 0.5f);

        imm_begin_texture_batch(g_asset_ui);
        if(cursor_in_bounds(x,y,w,h))
        {
            nkF32 scale = (is_mouse_button_pressed(MouseButton_Left)) ? 1.3f : 1.1f;
            render_item_ex(px,py, scale,scale, g_angles_big[APP_MAX_ANGLES-3], ATLAS_UI, ATLAS_UI_PAUSE_BODY, 1.0f);
        }
        else
        {
            render_item(px,py, ATLAS_UI, ATLAS_UI_PAUSE_BODY, 1.0f);
        }
        imm_end_texture_batch();
    }
}

static void pause_game(void)
{
    g_pause.paused = NK_TRUE;
}

static void unpause_game(void)
{
    g_pause.paused = NK_FALSE;
}

static nkBool is_game_paused(void)
{
    return g_pause.paused;
}

/*////////////////////////////////////////////////////////////////////////////*/
