/*////////////////////////////////////////////////////////////////////////////*/

#define PAUSE_BUTTON_X (SCREEN_WIDTH - 60.0f)
#define PAUSE_BUTTON_Y (                0.0f)
#define PAUSE_BUTTON_W (               60.0f)
#define PAUSE_BUTTON_H (               60.0f)

#define PAUSE_TITLE_YPOS   48.0f
#define PAUSE_RESUME_YPOS 160.0f
#define PAUSE_TITLE_SCALE   1.0f
#define PAUSE_RESUME_SCALE  0.5f

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
        sound_play(g_asset_sfx_page_flip[rng_int_range(0,10)], 0);
        g_pause.paused = !g_pause.paused;
    }

    // Do the pause button or menu logic.
    if(g_pause.paused)
    {
        // Update pause menu.
        if(update_text_button("RESUME", 168.0f, 0.7f))
        {
            sound_play(g_asset_sfx_page_flip[rng_int_range(0,10)], 0);
            g_pause.paused = NK_FALSE;
        }
    }
    else
    {
        nkF32 x = PAUSE_BUTTON_X;
        nkF32 y = PAUSE_BUTTON_Y;
        nkF32 w = PAUSE_BUTTON_W;
        nkF32 h = PAUSE_BUTTON_H;

        if(cursor_in_bounds(x,y,w,h) && is_mouse_button_pressed(MouseButton_Left))
        {
            sound_play(g_asset_sfx_page_flip[rng_int_range(0,10)], 0);
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
        render_bitmap_font_aligned("PAUSED", Alignment_Center, 48.0f, 1.0f, FontStyle_None);
        render_text_button("RESUME", 168.0f, 0.7f);
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
