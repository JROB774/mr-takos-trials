/*////////////////////////////////////////////////////////////////////////////*/

static nkNPAK g_npak;

static void load_assets_npak(void)
{
    #if defined(BUILD_NATIVE)
    if(!nk_npak_load(&g_npak, "assets.npak"))
        printf("Failed to load assets NPAK file!\n");
    #endif // BUILD_NATIVE
}

static void free_assets_npak(void)
{
    #if defined(BUILD_NATIVE)
    nk_npak_free(&g_npak);
    #endif // BUILD_NATIVE
}

static void load_all_assets(void)
{
    g_asset_background          = load_asset_texture("background.png",   SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_particle            = load_asset_texture("particle.png",     SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_ui                  = load_asset_texture("ui.png",           SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_font_big            = load_asset_texture("fontbig.png",      SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_font_lil            = load_asset_texture("fontlil.png",      SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_tako                = load_asset_texture("tako.png",         SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_simon               = load_asset_texture("simon.png",        SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_whack               = load_asset_texture("whack.png",        SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_match               = load_asset_texture("match.png",        SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_catch               = load_asset_texture("catch.png",        SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_debug_font_big      = load_asset_font   ("TexGyreCursor-BoldItalic.otf", 48.0f);
    g_asset_debug_font_lil      = load_asset_font   ("TexGyreCursor-BoldItalic.otf", 28.0f);
    g_asset_sfx_wrong_buzzer    = load_asset_sound  ("wrong_buzzer.wav");
    g_asset_sfx_alarm_clock     = load_asset_sound  ("alarm_clock.wav");
    g_asset_sfx_clock_ticking   = load_asset_sound  ("clock_ticking.wav");
    g_asset_sfx_office_stamp[0] = load_asset_sound  ("office_stamp_001.wav");
    g_asset_sfx_office_stamp[1] = load_asset_sound  ("office_stamp_002.wav");
    g_asset_sfx_office_stamp[2] = load_asset_sound  ("office_stamp_003.wav");
    g_asset_sfx_office_stamp[3] = load_asset_sound  ("office_stamp_004.wav");
    g_asset_sfx_office_stamp[4] = load_asset_sound  ("office_stamp_005.wav");
    g_asset_sfx_crowd_gasp      = load_asset_sound  ("crowd_gasp.wav");
    g_asset_sfx_success_ding    = load_asset_sound  ("success_ding.wav");
    g_asset_sfx_smack[0]        = load_asset_sound  ("smack_001.wav");
    g_asset_sfx_smack[1]        = load_asset_sound  ("smack_002.wav");
    g_asset_sfx_smack[2]        = load_asset_sound  ("smack_003.wav");
    g_asset_sfx_airhorn         = load_asset_sound  ("airhorn.wav");
    g_asset_sfx_mouse_click     = load_asset_sound  ("mouse_click.wav");
    g_asset_sfx_trumpet_fanfare = load_asset_sound  ("trumpet_fanfare.wav");
    g_asset_sfx_simon[0]        = load_asset_sound  ("simon_ba.wav");
    g_asset_sfx_simon[1]        = load_asset_sound  ("simon_be.wav");
    g_asset_sfx_simon[2]        = load_asset_sound  ("simon_bo.wav");
    g_asset_sfx_simon[3]        = load_asset_sound  ("simon_bu.wav");
    g_asset_sfx_page_flip[ 0]   = load_asset_sound  ("page_flip_001.wav");
    g_asset_sfx_page_flip[ 1]   = load_asset_sound  ("page_flip_002.wav");
    g_asset_sfx_page_flip[ 2]   = load_asset_sound  ("page_flip_003.wav");
    g_asset_sfx_page_flip[ 3]   = load_asset_sound  ("page_flip_004.wav");
    g_asset_sfx_page_flip[ 4]   = load_asset_sound  ("page_flip_005.wav");
    g_asset_sfx_page_flip[ 5]   = load_asset_sound  ("page_flip_006.wav");
    g_asset_sfx_page_flip[ 6]   = load_asset_sound  ("page_flip_007.wav");
    g_asset_sfx_page_flip[ 7]   = load_asset_sound  ("page_flip_008.wav");
    g_asset_sfx_page_flip[ 8]   = load_asset_sound  ("page_flip_009.wav");
    g_asset_sfx_page_flip[ 9]   = load_asset_sound  ("page_flip_010.wav");
    g_asset_sfx_page_flip[10]   = load_asset_sound  ("page_flip_011.wav");
    g_asset_sfx_paper_rustle[0] = load_asset_sound  ("paper_rustle_001.wav");
    g_asset_sfx_paper_rustle[1] = load_asset_sound  ("paper_rustle_002.wav");
    g_asset_sfx_gasp_small      = load_asset_sound  ("gasp_small.wav");
    g_asset_sfx_gasp_large      = load_asset_sound  ("gasp_large.wav");
    g_asset_sfx_explosion       = load_asset_sound  ("explosion.wav");
    g_asset_sfx_cat_meow        = load_asset_sound  ("cat_meow.wav");
    g_asset_sfx_shovel_whack    = load_asset_sound  ("shovel_whack.wav");
    g_asset_sfx_whack[0]        = load_asset_sound  ("whack_001.wav");
    g_asset_sfx_whack[1]        = load_asset_sound  ("whack_002.wav");
    g_asset_sfx_whack[2]        = load_asset_sound  ("whack_003.wav");
    g_asset_sfx_crumple[0]      = load_asset_sound  ("crumple_001.wav");
    g_asset_sfx_crumple[1]      = load_asset_sound  ("crumple_002.wav");
    g_asset_sfx_crumple[2]      = load_asset_sound  ("crumple_003.wav");
    g_asset_sfx_crumple[3]      = load_asset_sound  ("crumple_004.wav");
    g_asset_sfx_crumple[4]      = load_asset_sound  ("crumple_005.wav");
}

static void free_all_assets(void)
{
    texture_destroy(g_asset_background);
    texture_destroy(g_asset_particle);
    texture_destroy(g_asset_ui);
    texture_destroy(g_asset_font_big);
    texture_destroy(g_asset_font_lil);
    texture_destroy(g_asset_tako);
    texture_destroy(g_asset_simon);
    texture_destroy(g_asset_whack);
    texture_destroy(g_asset_match);
    texture_destroy(g_asset_catch);
    font_destroy   (g_asset_debug_font_big);
    font_destroy   (g_asset_debug_font_lil);
    sound_destroy  (g_asset_sfx_wrong_buzzer);
    sound_destroy  (g_asset_sfx_alarm_clock);
    sound_destroy  (g_asset_sfx_clock_ticking);
    sound_destroy  (g_asset_sfx_office_stamp[0]);
    sound_destroy  (g_asset_sfx_office_stamp[1]);
    sound_destroy  (g_asset_sfx_office_stamp[2]);
    sound_destroy  (g_asset_sfx_office_stamp[3]);
    sound_destroy  (g_asset_sfx_office_stamp[4]);
    sound_destroy  (g_asset_sfx_crowd_gasp);
    sound_destroy  (g_asset_sfx_success_ding);
    sound_destroy  (g_asset_sfx_smack[0]);
    sound_destroy  (g_asset_sfx_smack[1]);
    sound_destroy  (g_asset_sfx_smack[2]);
    sound_destroy  (g_asset_sfx_airhorn);
    sound_destroy  (g_asset_sfx_mouse_click);
    sound_destroy  (g_asset_sfx_trumpet_fanfare);
    sound_destroy  (g_asset_sfx_simon[0]);
    sound_destroy  (g_asset_sfx_simon[1]);
    sound_destroy  (g_asset_sfx_simon[2]);
    sound_destroy  (g_asset_sfx_simon[3]);
    sound_destroy  (g_asset_sfx_page_flip[ 0]);
    sound_destroy  (g_asset_sfx_page_flip[ 1]);
    sound_destroy  (g_asset_sfx_page_flip[ 2]);
    sound_destroy  (g_asset_sfx_page_flip[ 3]);
    sound_destroy  (g_asset_sfx_page_flip[ 4]);
    sound_destroy  (g_asset_sfx_page_flip[ 5]);
    sound_destroy  (g_asset_sfx_page_flip[ 6]);
    sound_destroy  (g_asset_sfx_page_flip[ 7]);
    sound_destroy  (g_asset_sfx_page_flip[ 8]);
    sound_destroy  (g_asset_sfx_page_flip[ 9]);
    sound_destroy  (g_asset_sfx_page_flip[10]);
    sound_destroy  (g_asset_sfx_paper_rustle[0]);
    sound_destroy  (g_asset_sfx_paper_rustle[1]);
    sound_destroy  (g_asset_sfx_gasp_small);
    sound_destroy  (g_asset_sfx_gasp_large);
    sound_destroy  (g_asset_sfx_explosion);
    sound_destroy  (g_asset_sfx_cat_meow);
    sound_destroy  (g_asset_sfx_shovel_whack);
    sound_destroy  (g_asset_sfx_whack[0]);
    sound_destroy  (g_asset_sfx_whack[1]);
    sound_destroy  (g_asset_sfx_whack[2]);
    sound_destroy  (g_asset_sfx_crumple[0]);
    sound_destroy  (g_asset_sfx_crumple[1]);
    sound_destroy  (g_asset_sfx_crumple[2]);
    sound_destroy  (g_asset_sfx_crumple[3]);
    sound_destroy  (g_asset_sfx_crumple[4]);
}

/*////////////////////////////////////////////////////////////////////////////*/

// Assets will look in the NPAK file first (if in a native build) and then
// fallback to looking to a file on disk if the file could not be found.

#define ASSET_PATH "assets/"

static Sound load_asset_sound(const nkChar* name)
{
    nkChar buffer[1024] = NK_ZERO_MEM;

    #if defined(BUILD_NATIVE)
    strcat(buffer, "audio/sound/");
    strcat(buffer, name);
    nkU64 file_size;
    void* file_data = nk_npak_get_file_data(&g_npak, buffer, &file_size);
    if(file_data)
        return sound_create_from_data(file_data, file_size);
    #endif // BUILD_NATIVE

    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "audio/sound/");
    strcat(buffer, name);

    return sound_create_from_file(buffer);
}

static Music load_asset_music(const nkChar* name)
{
    nkChar buffer[1024] = NK_ZERO_MEM;

    #if defined(BUILD_NATIVE)
    strcat(buffer, "audio/music/");
    strcat(buffer, name);
    nkU64 file_size;
    void* file_data = nk_npak_get_file_data(&g_npak, buffer, &file_size);
    if(file_data)
        return music_create_from_data(file_data, file_size);
    #endif // BUILD_NATIVE

    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "audio/music/");
    strcat(buffer, name);

    return music_create_from_file(buffer);
}

static Font load_asset_font(const nkChar* name, nkF32 px_height)
{
    nkChar buffer[1024] = NK_ZERO_MEM;

    #if defined(BUILD_NATIVE)
    strcat(buffer, "fonts/");
    strcat(buffer, name);
    nkU64 file_size;
    void* file_data = nk_npak_get_file_data(&g_npak, buffer, &file_size);
    if(file_data)
        return font_create(file_data, NK_FALSE, px_height);
    #endif // BUILD_NATIVE

    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "fonts/");
    strcat(buffer, name);

    nkFileContent file_content;
    if(!nk_read_file_content(&file_content, buffer, nkFileReadMode_Binary))
        fatal_error("Failed to load font from file: %s", buffer);
    Font font = font_create(file_content.data, NK_TRUE, px_height);
    return font;
}

static Texture load_asset_texture(const nkChar* name, SamplerFilter filter, SamplerWrap wrap)
{
    nkChar buffer[1024] = NK_ZERO_MEM;

    #if defined(BUILD_NATIVE)
    strcat(buffer, "textures/");
    strcat(buffer, name);
    nkU64 file_size;
    void* file_data = nk_npak_get_file_data(&g_npak, buffer, &file_size);
    if(file_data)
    {
        nkS32 w,h,bpp;
        nkU8* data = stbi_load_from_memory(file_data, file_size, &w,&h,&bpp, 4);
        if(!data)
            fatal_error("Failed to load texture from NPAK: %s", buffer);
        Texture texture = texture_create(w,h,4, data, filter, wrap);
        stbi_image_free(data);
        return texture;
    }
    #endif // BUILD_NATIVE

    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "textures/");
    strcat(buffer, name);

    nkS32 w,h,bpp;
    nkU8* data = stbi_load(buffer, &w,&h,&bpp, 4);
    if(!data)
        fatal_error("Failed to load texture from file: %s", buffer);
    Texture texture = texture_create(w,h,4, data, filter, wrap);
    stbi_image_free(data);
    return texture;
}

static Shader load_asset_shader(const nkChar* name)
{
    nkChar buffer[1024] = NK_ZERO_MEM;

    #if defined(BUILD_NATIVE)
    strcat(buffer, "shaders/");
    strcat(buffer, name);
    nkU64 file_size;
    void* file_data = nk_npak_get_file_data(&g_npak, buffer, &file_size);
    if(file_data)
        return shader_create(file_data, file_size);
    #endif // BUILD_NATIVE

    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "shaders/");
    strcat(buffer, name);

    nkFileContent file_content = NK_ZERO_MEM;
    if(!nk_read_file_content(&file_content, buffer, nkFileReadMode_Text))
        fatal_error("Failed to load shader from file: %s", buffer);
    Shader shader = shader_create(file_content.data, file_content.size);
    nk_free_file_content(&file_content);
    return shader;
}

/*////////////////////////////////////////////////////////////////////////////*/
