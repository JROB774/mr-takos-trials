/*////////////////////////////////////////////////////////////////////////////*/

typedef struct MiniGameTyper
{
    nkU32    current_word;
    nkChar** words;
    nkU32    word_count;
    nkChar   input[32];
    nkS32    combo;
}
MiniGameTyper;

static MiniGameTyper g_minigame_typer;

static void minigame_typer_select_new_word(void)
{
    // Generate a new word and make sure it's not the same as the previous word.
    nkU32 new_word = g_minigame_typer.current_word;
    while(new_word == g_minigame_typer.current_word)
        new_word = rng_int() % g_minigame_typer.word_count;
    g_minigame_typer.current_word = new_word;

    memset(g_minigame_typer.input, 0, sizeof(g_minigame_typer.input));
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
}

static void minigame_typer_quit(void)
{
    free(g_minigame_typer.words);
}

static void minigame_typer_start(void)
{
    cursor_set_type(CursorType_Arrow);

    minigame_typer_select_new_word();

    g_minigame_typer.combo = 0;
}

static void minigame_typer_end(void)
{
    // Nothing...
}

static void minigame_typer_update(nkF32 dt)
{
    if(game_is_playing() && !game_is_in_timeout())
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
                    sound_play(g_asset_sfx_office_stamp[rng_int_range(0,4)], 0);

                    if(toupper(text_input[i]) == toupper(current_word[index]))
                    {
                        g_minigame_typer.input[index] = tolower(text_input[i]);

                        g_minigame_typer.combo++;
                        g_gamestate.game_score += 1 * g_minigame_typer.combo;
                    }
                    else
                    {
                        game_display_failure((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));

                        g_minigame_typer.combo = 0;
                        g_gamestate.game_score -= 50;
                    }
                }
            }
        }

        // If the word is complete then advance!
        if(strcmp(g_minigame_typer.input, current_word) == 0)
        {
            game_display_success((SCREEN_WIDTH * 0.5f) + 80.0f, (SCREEN_HEIGHT - 32.0f));
            g_gamestate.game_score += 100; // Word bonus!
            minigame_typer_select_new_word();
        }
    }
}

static void minigame_typer_render(void)
{
    // Draw the current word.

    static const nkF32 WORD_SPACING = 3.0f;

    const nkChar* word = g_minigame_typer.words[g_minigame_typer.current_word];

    nkU32 word_length = strlen(word);

    nkF32 x = 0.0f;
    nkF32 y = 0.0f;
    nkF32 w = 0.0f;
    nkF32 h = 0.0f;

    // Calculate the bounds of the word so we can render it centered.
    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index = ATLAS_GAMETYPER_SOLID_A_SHADOW + (((toupper(word[i]) - 'A') * 2) + 1);
        w += ATLAS_GAMETYPER[index].clip_bounds.w + WORD_SPACING;
        h = nk_max(h, ATLAS_GAMETYPER[index].clip_bounds.h);
    }

    x = (SCREEN_WIDTH - w) * 0.5f;
    y = SCREEN_HEIGHT * 0.5f;

    imm_begin_texture_batch(g_asset_game_typer);

    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index = (g_minigame_typer.input[i] != word[i]) ? ATLAS_GAMETYPER_FADED_A_SHADOW : ATLAS_GAMETYPER_SOLID_A_SHADOW;;

        index += (((toupper(word[i]) - 'A') * 2) + 1);

        nkF32 angle = g_gamestate.angles_big[i];

        x += ((ATLAS_GAMETYPER[index].clip_bounds.w * 0.5f));

        render_item_ex(x,y, 1,1, angle, ATLAS_GAMETYPER, index, 1.0f);

        x += ((ATLAS_GAMETYPER[index].clip_bounds.w * 0.5f));
        x += WORD_SPACING;
    }

    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
