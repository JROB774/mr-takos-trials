/*////////////////////////////////////////////////////////////////////////////*/

#define CURSOR_IDLE_TIMEOUT 3.5f

typedef struct Cursor
{
    nkVec2     pos;
    nkF32      idle_time;
    CursorType type;
    nkBool     hidden;
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
    if(g_cursor.hidden) return; // Do not draw the cursor if it has been explicitly hidden.

    nkF32 p_off = (is_mouse_button_down(MouseButton_Left)) ? 1.0f : 0.0f;
    nkF32 s_off = (is_mouse_button_down(MouseButton_Left)) ? 0.6f : 1.0f;

    nkS32 index = ATLAS_UI_CURSOR_ARROW_SHADOW + ((g_cursor.type * 2) + 1);

    imm_begin_texture_batch(g_asset_ui);
    render_item(g_cursor.pos.x-p_off,g_cursor.pos.y-p_off, ATLAS_UI, index, s_off);
    imm_end_texture_batch();

    // Debug render the cursor's bounding box.
    #if 0
    ImmRect b = cursor_get_bounds();
    imm_rect_filled(b.x,b.y,b.w,b.h, (nkVec4){ 1.0f,0.0f,0.0f,0.5f });
    #endif
}

static void cursor_set_type(CursorType type)
{
    g_cursor.type = type;
}

static nkBool cursor_in_bounds(nkF32 x, nkF32 y, nkF32 w, nkF32 h)
{
    if(g_cursor.idle_time >= CURSOR_IDLE_TIMEOUT) return NK_FALSE;
    ImmRect b = cursor_get_bounds();
    return ((b.x < (x+w)) && ((b.x+b.h) > x) && (b.y < (y+h)) && ((b.y+b.h) > y));
}

static void cursor_hide(void)
{
    g_cursor.hidden = NK_TRUE;
}

static void cursor_show(void)
{
    g_cursor.hidden = NK_FALSE;
}

static ImmRect cursor_get_bounds(void)
{
    ImmRect bounds;
    bounds.x = g_cursor.pos.x - 16.0f;
    bounds.y = g_cursor.pos.y - 16.0f;
    bounds.w = 10.0f;
    bounds.h = 10.0f;
    return bounds;
}

/*////////////////////////////////////////////////////////////////////////////*/
