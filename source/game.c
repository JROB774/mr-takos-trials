/*////////////////////////////////////////////////////////////////////////////*/

static Texture   back_texture;
static Texture   tako_texture;
static Texture letter_texture;

static nkU32 seed;
static nkF32 timer;

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
      back_texture = load_asset_texture(  "back.png", SamplerFilter_Linear, SamplerWrap_Clamp);
      tako_texture = load_asset_texture(  "tako.png", SamplerFilter_Linear, SamplerWrap_Clamp);
    letter_texture = load_asset_texture("letter.png", SamplerFilter_Linear, SamplerWrap_Clamp);

    show_cursor(NK_FALSE);
}

static void game_quit(void)
{
    texture_destroy(  tako_texture);
    texture_destroy(  back_texture);
    texture_destroy(letter_texture);
}

static void game_update(nkF32 dt)
{
    timer += dt;
    if(timer >= 0.5f)
    {
        timer -= 0.5f;
        seed = clock();
    }
}

static void render_text_string(nkF32 start_x, nkF32 start_y, const nkChar* text)
{
    nkF32 x = start_x;
    nkF32 y = start_y;

    imm_begin_texture_batch(letter_texture);
    for(nkS32 i=0,n=strlen(text); i<n; ++i)
    {
        nkF32 angle = random_float_range(-0.22f, 0.22f);
        nkS32 index = (toupper(text[i]) - 'A') * 2;
        if(i != 0)
            x += ((ATLAS_LETTER[index+1].clip_bounds.w * 0.5f));
        imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,0.6f });
        imm_atlas_batched_ex(x+4,y+4, 1,1, angle, NULL, &ATLAS_LETTER[index]);
        imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,1.0f });
        imm_atlas_batched_ex(x,y, 1,1, angle, NULL, &ATLAS_LETTER[index+1]);
        x += ((ATLAS_LETTER[index+1].clip_bounds.w * 0.5f)) + 3.0f;
    }
    imm_end_texture_batch();
}

static void render_tako(nkF32 x, nkF32 y)
{
    nkF32 angle = random_float_range(-0.22f, 0.22f);

    imm_begin_texture_batch(tako_texture);
    imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,0.6f });
    imm_atlas_batched_ex(x+4.0f,y+4.0f, 1,1, angle, NULL, &ATLAS_TAKO[ATLAS_TAKO_SHADOW]);
    imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,1.0f });
    imm_atlas_batched_ex(x,y, 1,1, angle, NULL, &ATLAS_TAKO[ATLAS_TAKO_BODY]);
    imm_end_texture_batch();
}

static void game_render(void)
{
    clear_screen_v(NK_V4_BLACK);

    set_blend_mode(BlendMode_Alpha);

    nkF32 cx = SCREEN_WIDTH * 0.5f;
    nkF32 cy = SCREEN_HEIGHT * 0.5f;

    imm_texture(back_texture, cx,cy, NULL);

    srand(seed);

    /*
    srand(56673286);

    imm_begin_texture_batch(tako_texture);
    #if 1
    for(nkS32 i=0; i<100; ++i)
    {
        nkF32 x = random_float_range(0, SCREEN_WIDTH);
        nkF32 y = random_float_range(0, SCREEN_HEIGHT);
        nkF32 angle = random_float_range(0, NK_TAU);

        imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,0.6f });
        imm_atlas_batched_ex(x+4.0f,y+4.0f, 1.0f,1.0f, angle, NULL, &ATLAS_TAKO[ATLAS_TAKO_SHADOW]);
        imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,1.0f });
        imm_atlas_batched_ex(x,y, 1.0f,1.0f, angle, NULL, &ATLAS_TAKO[ATLAS_TAKO_BODY]);
    }
    #else
    imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,0.6f });
    imm_atlas_batched(cx+4.0f,cy+4.0f, &ATLAS_TAKO[ATLAS_TAKO_SHADOW]);
    imm_set_texture_color((nkVec4){ 1.0f,1.0f,1.0f,1.0f });
    imm_atlas_batched(cx,cy, &ATLAS_TAKO[ATLAS_TAKO_BODY]);
    #endif
    imm_end_texture_batch();
    */

    render_text_string(135,cy-35, "Hello");
    render_text_string(120,cy+35, "World");

    render_tako(20,cy);
    render_tako(460,cy);
}

/*////////////////////////////////////////////////////////////////////////////*/
