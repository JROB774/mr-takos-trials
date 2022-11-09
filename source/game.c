/*////////////////////////////////////////////////////////////////////////////*/

static Texture back_texture;
static Texture tako_texture;
static Texture test_texture;

static nkS32 frame;
static nkF32 frame_timer;

static void game_init(void)
{
    back_texture = load_asset_texture( "back.png", SamplerFilter_Nearest, SamplerWrap_Clamp);
    tako_texture = load_asset_texture( "tako.png", SamplerFilter_Nearest, SamplerWrap_Clamp);
    test_texture = load_asset_texture("test0.png", SamplerFilter_Nearest, SamplerWrap_Clamp);

    show_cursor(NK_FALSE);
}

static void game_quit(void)
{
    texture_destroy(test_texture);
    texture_destroy(tako_texture);
    texture_destroy(back_texture);
}

static void game_update(nkF32 dt)
{
    frame_timer += dt;
    if(frame_timer >= 0.33f)
    {
        frame_timer -= 0.33f;
        frame = ((frame + 1) % NK_ARRAY_SIZE(ATLAS_TEST0));
    }
}

static void game_render(void)
{
    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    nkF32 cx = SCREEN_WIDTH * 0.5f;
    nkF32 cy = SCREEN_HEIGHT * 0.5f;

    imm_texture(back_texture, cx,cy, NULL);
    imm_texture(tako_texture, cx,cy, NULL);

    imm_rect_filled(cx-64,cy-64,128,128, NK_V4_BLUE);

    nkF32 tx = cx + ATLAS_TEST0[frame].offset_x;
    nkF32 ty = cy + ATLAS_TEST0[frame].offset_y;

    imm_texture(test_texture, tx,ty, &ATLAS_TEST0[frame].bounds);
}

/*////////////////////////////////////////////////////////////////////////////*/
