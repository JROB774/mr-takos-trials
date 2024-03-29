/*////////////////////////////////////////////////////////////////////////////*/

#define PAUSE_BUTTON_X (SCREEN_WIDTH - 60.0f)
#define PAUSE_BUTTON_Y (                0.0f)
#define PAUSE_BUTTON_W (               60.0f)
#define PAUSE_BUTTON_H (               60.0f)

#define PAUSE_TEXT_SCALE 0.7f

typedef struct PauseState
{
    nkBool paused;
}
PauseState;

static PauseState g_pause;

static void pause_update(nkF32 dt)
{
    // Cannot be paused in any other state but the game, also cannot pause after the timer is done.
    if(g_appstate != AppState_Game || game_get_timer() <= 0.0f)
    {
        g_pause.paused = NK_FALSE;
        return;
    }

    nkBool prev_paused = g_pause.paused;

    // Check for the pause button hotkey.
    if(is_key_pressed(KeyCode_Escape))
    {
        change_page();
        g_pause.paused = !g_pause.paused;
    }

    // Do the pause button or menu logic.
    if(g_pause.paused)
    {
        // Update pause menu.
        nkF32 y = 168.0f;
        if(update_simple_button_aligned("RESUME", y, PAUSE_TEXT_SCALE, Alignment_Left))
        {
            change_page();
            g_pause.paused = NK_FALSE;
        }

        y += bitmap_font_line_advance(FontSize_Big, PAUSE_TEXT_SCALE);

        if(update_simple_button_aligned("MENU", y, PAUSE_TEXT_SCALE, Alignment_Left))
        {
            if(g_gamestate.alarm_sound_ref != INVALID_SOUND_REF)
                sound_stop(g_gamestate.alarm_sound_ref);
            change_page();
            g_pause.paused = NK_FALSE;
            g_appstate = AppState_Menu;
        }
    }
    else
    {
        nkF32 x = PAUSE_BUTTON_X;
        nkF32 y = PAUSE_BUTTON_Y;
        nkF32 w = PAUSE_BUTTON_W;
        nkF32 h = PAUSE_BUTTON_H;

        if(cursor_in_bounds(x,y,w,h) && !cursor_is_hidden() && is_mouse_button_pressed(MouseButton_Left))
        {
            change_page();
            g_pause.paused = NK_TRUE;
        }
    }

    // Pause/resume the timer sounds when entering/exiting pause.
    if(prev_paused != g_pause.paused && g_appstate == AppState_Game)
    {
        if(g_gamestate.tick_sound_ref != INVALID_SOUND_REF)
        {
            if(g_pause.paused) sound_pause(g_gamestate.tick_sound_ref);
            else sound_resume(g_gamestate.tick_sound_ref);
        }
        if(g_gamestate.alarm_sound_ref != INVALID_SOUND_REF)
        {
            if(g_pause.paused) sound_pause(g_gamestate.alarm_sound_ref);
            else sound_resume(g_gamestate.alarm_sound_ref);
        }
    }
}

static void pause_render(void)
{
    // Cannot be paused in any other state but the game, also cannot pause after the timer is done.
    if(g_appstate != AppState_Game || game_get_timer() <= 0.0f)
    {
        g_pause.paused = NK_FALSE;
        return;
    }

    // Do the pause button or menu logic.
    if(g_pause.paused)
    {
        // Render pause menu.
        render_bitmap_font_aligned(FontSize_Big, "PAUSED", Alignment_Center, 48.0f, 1.0f, FontStyle_None);

        nkF32 y = 168.0f;

        render_simple_button_aligned("RESUME", y, PAUSE_TEXT_SCALE, Alignment_Left);
        y += bitmap_font_line_advance(FontSize_Big, PAUSE_TEXT_SCALE);
        render_simple_button_aligned("MENU", y, PAUSE_TEXT_SCALE, Alignment_Left);

        imm_begin_texture_batch(g_asset_tako);
        render_item_ex(380.0f,188.0f, 1,1, g_angles_lil[APP_MAX_ANGLES-1], ATLAS_TAKO, ATLAS_TAKO_PAUSE_BODY, 1.0f);
        imm_end_texture_batch();
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
        if(cursor_in_bounds(x,y,w,h) && !cursor_is_hidden())
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
