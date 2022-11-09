/*////////////////////////////////////////////////////////////////////////////*/

static Texture back_texture;
static Texture tako_texture;

static Font test_font;

static void game_init(void)
{
    back_texture = load_asset_texture("back.png", SamplerFilter_Nearest, SamplerWrap_Clamp);
    tako_texture = load_asset_texture("tako.png", SamplerFilter_Nearest, SamplerWrap_Clamp);

    test_font = load_asset_font("mabook.ttf", 48.0f);

    show_cursor(NK_FALSE);
}

static void game_quit(void)
{
    font_destroy(test_font);

    texture_destroy(tako_texture);
    texture_destroy(back_texture);
}

static void game_update(nkF32 dt)
{
    // @Incomplete: ...
}

static void game_render(void)
{
    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    nkF32 cx = SCREEN_WIDTH * 0.5f;
    nkF32 cy = SCREEN_HEIGHT * 0.5f;

    imm_texture(back_texture, cx,cy, NULL);
    imm_texture(tako_texture, cx,cy, NULL);
}

/*////////////////////////////////////////////////////////////////////////////*/
