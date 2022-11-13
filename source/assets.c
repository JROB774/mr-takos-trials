/*////////////////////////////////////////////////////////////////////////////*/

static void load_all_assets(void)
{
    g_asset_background = load_asset_texture("back.png", SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_letter     = load_asset_texture("letter.png", SamplerFilter_Linear, SamplerWrap_Clamp);
    g_asset_wordlist   = load_asset_text   ("wordlist.txt");
}

static void free_all_assets(void)
{
    texture_destroy(g_asset_background);
    texture_destroy(g_asset_letter);
    free           (g_asset_wordlist);
}

/*////////////////////////////////////////////////////////////////////////////*/

#if defined(BUILD_NATIVE)
#define ASSET_PATH "assets/"
#endif // BUILD_NATIVE
#if defined(BUILD_WEB)
#define ASSET_PATH ""
#endif // BUILD_WEB

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
