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
    nkVec2 mouse = get_screen_mouse_pos();
    if(nk_v2_equ(g_cursor.pos, mouse))
    {
        g_cursor.idle_time += dt;
        return; // Return early...
    }

    g_cursor.pos.x = mouse.x;
    g_cursor.pos.y = mouse.y;

    g_cursor.idle_time = 0.0f;
}

static void cursor_render(void)
{
    if(g_cursor.idle_time >= CURSOR_IDLE_TIMEOUT) return; // Hide the cursor after a bit of inactivity.
    imm_begin_texture_batch(g_asset_ui);
    render_item(g_cursor.pos.x,g_cursor.pos.y, ATLAS_UI, ATLAS_UI_CURSOR_BODY);
    imm_end_texture_batch();
}

static nkBool cursor_in_bounds(nkF32 x, nkF32 y, nkF32 w, nkF32 h)
{
    // @Incomplete: Check a box for more lenience...
    return ((g_cursor.pos.x >= x) && (g_cursor.pos.y >= y) &&
            (g_cursor.pos.x < (x+w)) && (g_cursor.pos.y < (y+h)));
}

/*////////////////////////////////////////////////////////////////////////////*/
