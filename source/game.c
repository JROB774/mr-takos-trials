/*////////////////////////////////////////////////////////////////////////////*/

static Texture back_texture;
static Texture tako_texture;

static Font test_font;

static nkF32 random_float(void)
{
    return (NK_CAST(nkF32,rand()) / NK_CAST(nkF32,RAND_MAX));
}
static nkF32 random_float_range(nkF32 min, nkF32 max)
{
    return (min + NK_CAST(nkF32,rand()) / NK_CAST(nkF32,RAND_MAX/(max-min)));
}

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

    /*
    static nkU32 seed = 673561296;

    if(is_key_down(KeyCode_F5))
    {
        seed = clock();
    }

    srand(seed);

    imm_begin_texture_batch(sponge_texture);
    for(nkS32 i=0; i<1024; ++i)
    {
        nkF32 px = random_float_range(0,SCREEN_WIDTH);
        nkF32 py = random_float_range(0,SCREEN_HEIGHT);
        nkF32 sc = random_float_range(0.025f, 0.5f);
        nkF32 ar = random_float_range(0.0f, NK_TAU);

        nkVec4 color;
        color.r = random_float();
        color.g = random_float();
        color.b = random_float();
        color.a = random_float();

        imm_set_texture_color(color);
        imm_texture_batched_ex(px,py, sc,sc, ar, NULL, NULL);
    }
    imm_end_texture_batch();

    const nkChar* text = "Hello World!!!";

    nkVec2 tb = font_get_text_bounds(test_font, text);

    nkF32 tx = ((SCREEN_WIDTH - tb.x) * 0.5f);
    nkF32 ty = ((SCREEN_HEIGHT + tb.y) * 0.5f);

    font_draw_text(test_font, tx+4.0f,ty+4.0f, text, (nkVec4){ 0.1f,0.1f,0.0f,1.0f });
    font_draw_text(test_font, tx,ty, text, (nkVec4){ 1.0f,1.0f,0.4f,1.0f });

    // imm_rect_outline(tx, aabb.y0, aabb.x1-aabb.x0, aabb.y1-aabb.y0, NK_V4_RED);

    nkVec2 mp = get_screen_mouse_pos();
    ImmRect cursor_clip = { 0,0,32,32 };
    if(is_mouse_button_down(MouseButton_Left))
        cursor_clip.x += 32.0f;
    imm_set_texture_color((nkVec4){ 1,1,1,1 });
    imm_texture(cursor_texture, mp.x, mp.y, &cursor_clip);
    */
}

/*////////////////////////////////////////////////////////////////////////////*/
