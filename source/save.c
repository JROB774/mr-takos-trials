/*////////////////////////////////////////////////////////////////////////////*/

static const nkChar* get_save_file_name(void)
{
    static nkChar file_name_buffer[1024] = NK_ZERO_MEM;
    #if defined(BUILD_NATIVE)
    strcpy(file_name_buffer, get_base_path());
    #endif// BUILD_NATIVE
    #if defined(BUILD_WEB)
    strcpy(file_name_buffer, "/TAKO/");
    #endif // BUILD_WEB
    strcat(file_name_buffer, "save.dat");
    return file_name_buffer;
}

static void save_game_data(void)
{
    const nkChar* file_name = get_save_file_name();

    g_save.version = SAVE_FILE_VERSION;
    g_save.fourcc = SAVE_FILE_FOURCC;

    FILE* file = fopen(file_name, "wb");
    if(!file)
    {
        user_error("Failed to save game data!");
        return;
    }
    fwrite(&g_save, sizeof(g_save), 1, file);
    fclose(file);

    #if defined(BUILD_WEB)
    EM_ASM(FS.syncfs(function(err) { assert(!err); }));
    #endif // BULD_WEB
}

static void load_game_data(void)
{
    const nkChar* file_name = get_save_file_name();
    if(!nk_file_exists(file_name)) return;

    FILE* file = fopen(file_name, "rb");
    if(!file)
    {
        user_error("Failed to load game data!");
        return;
    }
    fread(&g_save, sizeof(g_save), 1, file);
    fclose(file);

    #if defined(BUILD_WEB)
    EM_ASM(FS.syncfs(function(err) { assert(!err); }));
    #endif // BULD_WEB

    // If the save file is an invalid version or format then do not load it.
    if(g_save.version != SAVE_FILE_VERSION || g_save.fourcc != SAVE_FILE_FOURCC)
    {
        user_error("Invalid save file cannot be loaded!");
        memset(&g_save, 0, sizeof(g_save));
    }
}

/*////////////////////////////////////////////////////////////////////////////*/
