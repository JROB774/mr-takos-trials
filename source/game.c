/*////////////////////////////////////////////////////////////////////////////*/

Texture sponge_texture;

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
    sponge_texture = imm_load_texture_from_file("sponge.png", SamplerFilter_Nearest, SamplerWrap_Clamp);

    enable_alpha_blend();
}

static void game_quit(void)
{
    texture_destroy(sponge_texture);
}

static void game_update(nkF32 dt)
{
    // @Incomplete: ...
}

static void game_render(void)
{
    clear_screen_f(0.1f, 0.2f, 0.3f, 1.0f);

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
}

/*////////////////////////////////////////////////////////////////////////////*/
