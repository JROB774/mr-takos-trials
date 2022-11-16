/*////////////////////////////////////////////////////////////////////////////*/

static void load_all_assets(void)
{
    g_asset_back_paper          = load_asset_texture("backpaper.png",    SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_back_vignette       = load_asset_texture("backvignette.png", SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_particle            = load_asset_texture("particle.png",     SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_ui                  = load_asset_texture("ui.png",           SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_game_typer          = load_asset_texture("gametyper.png",    SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_game_simon          = load_asset_texture("gamesimon.png",    SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_wordlist            = load_asset_text   ("wordlist.txt");
    g_asset_font_big            = load_asset_font   ("TexGyreCursor-BoldItalic.otf", 48.0f);
    g_asset_font_lil            = load_asset_font   ("TexGyreCursor-BoldItalic.otf", 28.0f);
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
}

static void free_all_assets(void)
{
    texture_destroy(g_asset_back_paper);
    texture_destroy(g_asset_back_vignette);
    texture_destroy(g_asset_particle);
    texture_destroy(g_asset_ui);
    texture_destroy(g_asset_game_typer);
    texture_destroy(g_asset_game_simon);
    free           (g_asset_wordlist);
    font_destroy   (g_asset_font_big);
    font_destroy   (g_asset_font_lil);
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
}

/*////////////////////////////////////////////////////////////////////////////*/

#define ASSET_PATH "assets/"

static Sound load_asset_sound(const nkChar* name)
{
    nkChar buffer[1024] = NK_ZERO_MEM;
    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "audio/sound/");
    strcat(buffer, name);

    return sound_create(buffer);
}

static Music load_asset_music(const nkChar* name)
{
    nkChar buffer[1024] = NK_ZERO_MEM;
    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "audio/music/");
    strcat(buffer, name);

    return music_create(buffer);
}

static nkChar* load_asset_text(const nkChar* name)
{
    nkChar buffer[1024] = NK_ZERO_MEM;
    strcpy(buffer, get_base_path());
    strcat(buffer, ASSET_PATH);
    strcat(buffer, "strings/");
    strcat(buffer, name);

    nkFileContent file_content;
    if(!nk_read_file_content(&file_content, buffer, nkFileReadMode_Text))
        fatal_error("Failed to load text from file: %s", buffer);
    return file_content.data;
}

static Font load_asset_font(const nkChar* name, nkF32 px_height)
{
    nkChar buffer[1024] = NK_ZERO_MEM;
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
