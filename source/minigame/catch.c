/*////////////////////////////////////////////////////////////////////////////*/

#define CATCH_BASKET_WIDTH  40.0f
#define CATCH_BASKET_HEIGHT 80.0f

typedef struct MiniGameCatch
{
    nkVec2 basket_pos;
    nkBool basket_grabbed;
}
MiniGameCatch;

static MiniGameCatch g_minigame_catch;

static void minigame_catch_start(void)
{
    cursor_set_type(CursorType_Grabber);

    g_minigame_catch.basket_pos.x = SCREEN_WIDTH * 0.5f;
    g_minigame_catch.basket_pos.y = SCREEN_HEIGHT - 72.0f;

    g_minigame_catch.basket_grabbed = NK_FALSE;
}

static void minigame_catch_end(void)
{
    // @Incomplete: ...
}

static void minigame_catch_update(nkF32 dt)
{
    if(game_is_playing())
    {
        if(!g_minigame_catch.basket_grabbed)
        {
            // Grab the basket if the cursor collides.
            if(is_mouse_button_pressed(MouseButton_Left))
            {
                nkF32 bx = g_minigame_catch.basket_pos.x - (CATCH_BASKET_WIDTH * 0.5f);
                nkF32 by = g_minigame_catch.basket_pos.y - (CATCH_BASKET_HEIGHT * 0.5f);
                nkF32 bw = CATCH_BASKET_WIDTH;
                nkF32 bh = CATCH_BASKET_HEIGHT;

                if(cursor_in_bounds(bx,by,bw,bh))
                {
                    g_minigame_catch.basket_grabbed = NK_TRUE;
                    cursor_hide();
                    set_mouse_to_relative(NK_TRUE);
                }
            }
        }
        else
        {
            // Release the basket.
            if(is_mouse_button_released(MouseButton_Left))
            {
                g_minigame_catch.basket_grabbed = NK_FALSE;
                cursor_show();
                set_mouse_to_relative(NK_FALSE);
            }

            // Make the basket follow the mouse horizontally.
            g_minigame_catch.basket_pos.x += get_relative_screen_mouse_pos().x;
        }
    }
    else
    {
        // Make sure we exit relative mouse mode and show the cursor.
        set_mouse_to_relative(NK_FALSE);
        cursor_show();
    }
}

static void minigame_catch_render(void)
{
    imm_begin_texture_batch(g_asset_catch);

    // Draw the basket.
    nkF32 bx = g_minigame_catch.basket_pos.x;
    nkF32 by = g_minigame_catch.basket_pos.y;
    nkF32 angle = (g_minigame_catch.basket_grabbed) ? g_angles_lil[0] : 0.0f;
    render_item_ex(bx,by, 1,1, angle, ATLAS_CATCH, ATLAS_CATCH_BASKET_BODY, 1.0f);

    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
