/*////////////////////////////////////////////////////////////////////////////*/

#define CURSOR_IDLE_TIMEOUT 3.5f

typedef struct Cursor
{
    nkVec2 pos;
    nkF32  idle_time;
}
Cursor;

static Cursor g_cursor;

static void cursor_update(nkF32 dt)
{
    // Play click sound.
    if(is_mouse_button_pressed(MouseButton_Left))
    {
        sound_play(g_asset_sfx_mouse_click, 0);
    }

    // Update idle timer.
    nkVec2 mouse = get_screen_mouse_pos();

    mouse.x = roundf(mouse.x);
    mouse.y = roundf(mouse.y);

    if(nk_v2_equ(g_cursor.pos, mouse) && !is_mouse_button_down(MouseButton_Left))
    {
        g_cursor.idle_time += dt;
        return; // Return early...
    }

    // Update position.
    g_cursor.pos.x = mouse.x;
    g_cursor.pos.y = mouse.y;

    g_cursor.idle_time = 0.0f;
}

static void cursor_render(void)
{
    if(g_cursor.idle_time >= CURSOR_IDLE_TIMEOUT) return; // Hide the cursor after a bit of inactivity.

    nkF32 p_off = (is_mouse_button_down(MouseButton_Left)) ? 1.0f : 0.0f;
    nkF32 s_off = (is_mouse_button_down(MouseButton_Left)) ? 0.6f : 1.0f;

    imm_begin_texture_batch(g_asset_ui);
    render_item(g_cursor.pos.x-p_off,g_cursor.pos.y-p_off, ATLAS_UI, ATLAS_UI_CURSOR_BODY, s_off);
    imm_end_texture_batch();
}

static nkBool cursor_in_bounds(nkF32 x, nkF32 y, nkF32 w, nkF32 h)
{
    nkF32 cx = g_cursor.pos.x - 16.0f;
    nkF32 cy = g_cursor.pos.y - 16.0f;
    nkF32 cw = 10.0f;
    nkF32 ch = 10.0f;

    return ((cx < (x+w)) && ((cx+ch) > x) && (cy < (y+h)) && ((cy+ch) > y));
}

static void cursor_hide(void)
{
    g_cursor.idle_time = CURSOR_IDLE_TIMEOUT;
}

static void cursor_show(void)
{
    g_cursor.idle_time = 0.0f;
}

/*////////////////////////////////////////////////////////////////////////////*/
