/*////////////////////////////////////////////////////////////////////////////*/

typedef struct MiniGameTyper
{
    nkU32    current_word;
    nkChar** words;
    nkU32    word_count;
    nkChar   input[32];
}
MiniGameTyper;

static MiniGameTyper g_minigame_typer;

static void minigame_typer_select_new_word(void)
{
    g_minigame_typer.current_word = rand() % g_minigame_typer.word_count;
    memset(g_minigame_typer.input, 0, sizeof(g_minigame_typer.input));
}

static void minigame_typer_draw_word(const nkU32 word_index)
{
    static const nkVec4 SHADOW_COLOR = { 1.0f,1.0f,1.0f,0.6f };
    static const nkVec4 BODY_COLOR = { 1.0f,1.0f,1.0f,1.0f };

    static const nkF32 WORD_SPACING = 3.0f;

    static const nkVec2 SHADOW_OFFSET = { 4.0f,4.0f };

    const nkChar* word = g_minigame_typer.words[word_index];

    nkU32 word_length = strlen(word);

    nkF32 x = 0.0f;
    nkF32 y = 0.0f;
    nkF32 w = 0.0f;
    nkF32 h = 0.0f;

    // Calculate the bounds of the word so we can render it centered.
    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index = ((toupper(word[i]) - 'A') * 2) + 1;
        w += ATLAS_LETTER[index].clip_bounds.w + WORD_SPACING;
        h = nk_max(h, ATLAS_LETTER[index].clip_bounds.h);
    }

    x = (SCREEN_WIDTH - w) * 0.5f;
    y = SCREEN_HEIGHT * 0.5f;

    imm_begin_texture_batch(g_asset_letter);

    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index_shadow = ((toupper(word[i]) - 'A') * 2) + 0;
        nkS32 index_body = ((toupper(word[i]) - 'A') * 2) + 1;

        x += ((ATLAS_LETTER[index_body].clip_bounds.w * 0.5f));

        nkVec4 shadow_color = SHADOW_COLOR;
        nkVec4 body_color = BODY_COLOR;

        if(g_minigame_typer.input[i] != word[i])
        {
            shadow_color.a *= 0.5f;
            body_color.a *= 0.5f;
        }

        nkF32 bx = x;
        nkF32 by = y;
        nkF32 sx = x + SHADOW_OFFSET.x;
        nkF32 sy = y + SHADOW_OFFSET.y;

        imm_atlas_batched_ex(sx,sy, 1,1, 0, NULL, &ATLAS_LETTER[index_shadow], shadow_color);
        imm_atlas_batched_ex(bx,by, 1,1, 0, NULL, &ATLAS_LETTER[index_body], body_color);

        x += ((ATLAS_LETTER[index_body].clip_bounds.w * 0.5f));
        x += WORD_SPACING;
    }

    imm_end_texture_batch();
}

static void minigame_typer_init(void)
{
    // Load the raw list of words into an array of strings for easy access.
    g_minigame_typer.words = malloc(512*sizeof(nkChar*));
    if(!g_minigame_typer.words) fatal_error("Failed to allocate words!");

    nkU32 start = 0;
    nkU32 length = 0;

    for(nkU32 i=0,n=strlen(g_asset_wordlist); i<n; ++i)
    {
        nkChar c = g_asset_wordlist[i];
        if((c == '\n' || c == '\r'))
        {
            if(length > 0)
            {
                nkU32 wordsize = (length+1)*sizeof(nkChar);
                nkChar* word = malloc(wordsize);
                if(!word) fatal_error("Failed to allocate word!");
                memset(word, 0, wordsize);
                strncpy(word, g_asset_wordlist+start, length);
                g_minigame_typer.words[g_minigame_typer.word_count] = word;
                g_minigame_typer.word_count++;
            }
            start = i+1;
            length = 0;
        }
        else
        {
            length++;
        }
    }

    minigame_typer_select_new_word();
}

static void minigame_typer_quit(void)
{
    free(g_minigame_typer.words);
}

static void minigame_typer_update(nkF32 dt)
{
    // Compare the current text input with what the user has left to type.
    nkChar* current_word = g_minigame_typer.words[g_minigame_typer.current_word];
    nkChar* text_input = get_current_text_input();
    if(text_input && strlen(text_input) > 0)
    {
        for(nkU32 i=0,n=strlen(text_input); i<n; ++i)
        {
            nkU32 index = strlen(g_minigame_typer.input);
            if(index < strlen(current_word))
            {
                if(toupper(text_input[i]) == toupper(current_word[index]))
                {
                    g_minigame_typer.input[index] = tolower(text_input[i]);
                }
            }
        }
    }
    // If the word is complete then advance!
    if(strcmp(g_minigame_typer.input, current_word) == 0)
    {
        minigame_typer_select_new_word();
    }
}

static void minigame_typer_render(void)
{
    minigame_typer_draw_word(g_minigame_typer.current_word);
}

/*////////////////////////////////////////////////////////////////////////////*/
