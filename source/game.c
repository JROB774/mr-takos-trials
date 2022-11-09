/*////////////////////////////////////////////////////////////////////////////*/

static Texture back_texture;
static Texture tako_texture;
static Texture test_texture;

static nkS32 frame;
static nkF32 frame_timer;
static nkF32 frame_angle;

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
    frame_angle += dt;
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

    nkMat4 model = nk_m4_identity();
    model = nk_translate(model, (nkVec3){ cx,cy,0 });
    model = nk_rotate(model, (nkVec3){ 0,0,1 }, frame_angle * 2.5f);
    imm_set_model(model);
    imm_rect_filled(-64,-64,128,128, NK_V4_BLUE);

    nkVec2 anchor = NK_ZERO_MEM;

    anchor.x = ATLAS_TEST0[frame].box.w * 0.5f;
    anchor.y = ATLAS_TEST0[frame].box.h * 0.5f;

    nkF32 nx = anchor.x / ATLAS_TEST0[frame].box.w;
    nkF32 ny = anchor.y / ATLAS_TEST0[frame].box.h;

    nkF32 offx = ATLAS_TEST0[frame].box.x + (nx * ATLAS_TEST0[frame].box.w);
    nkF32 offy = ATLAS_TEST0[frame].box.y + (ny * ATLAS_TEST0[frame].box.h);

    offx = offx - (nx * ATLAS_TEST0[frame].original_size_x);
    offy = offy - (ny * ATLAS_TEST0[frame].original_size_y);

    anchor.x -= offx;
    anchor.y -= offy;

    imm_texture_ex(test_texture, cx,cy, 1,1, frame_angle * 2.5f, &anchor, &ATLAS_TEST0[frame].clip);
}

/*////////////////////////////////////////////////////////////////////////////*/
