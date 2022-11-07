/*////////////////////////////////////////////////////////////////////////////*/

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
    strcat(buffer, SHADER_PATH);
    strcat(buffer, name);

    nkFileContent file_content = NK_ZERO_MEM;
    if(!nk_read_file_content(&file_content, buffer, nkFileReadMode_Text))
        fatal_error("Failed to load shader from file: %s", buffer);
    Shader shader = shader_create(file_content.data, file_content.size);
    nk_free_file_content(&file_content);
    return shader;
}

/*////////////////////////////////////////////////////////////////////////////*/
