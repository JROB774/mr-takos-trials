/*////////////////////////////////////////////////////////////////////////////*/

#define LETTER_MIN_ANGLE -0.4f
#define LETTER_MAX_ANGLE  0.4f

#define TYPING_COOLDOWN 0.8f

typedef struct MiniGameTyper
{
    nkU32    current_word;
    nkChar** words;
    nkU32    word_count;
    nkChar   input[32];
    nkF32    angles[32];
    nkF32    timer;
    nkF32    cooldown;
}
MiniGameTyper;

static MiniGameTyper g_minigame_typer;

static void minigame_typer_select_new_word(void)
{
    // Generate a new word and make sure it's not the same as the previous word.
    nkU32 new_word = g_minigame_typer.current_word;
    while(new_word == g_minigame_typer.current_word)
        new_word = rng_int(&g_rng_l) % g_minigame_typer.word_count;
    g_minigame_typer.current_word = new_word;

    memset(g_minigame_typer.input, 0, sizeof(g_minigame_typer.input));
}

static void minigame_typer_draw_word(const nkU32 word_index)
{
    static const nkF32 WORD_SPACING = 3.0f;

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
        nkS32 index = ((toupper(word[i]) - 'A') * 2) + 1;
        nkF32 angle = g_minigame_typer.angles[i];

        x += ((ATLAS_LETTER[index].clip_bounds.w * 0.5f));

        render_item_ex(x,y, 1,1, angle, ATLAS_LETTER, index, (g_minigame_typer.input[i] != word[i]) ? 0.5f : 1.0f);

        x += ((ATLAS_LETTER[index].clip_bounds.w * 0.5f));
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

    // Setup the initial rendering angles;
    for(nkS32 i=0,n=NK_ARRAY_SIZE(g_minigame_typer.angles); i<n; ++i)
    {
        g_minigame_typer.angles[i] = rng_num_range(&g_rng_v, LETTER_MIN_ANGLE,LETTER_MAX_ANGLE);
    }

    g_minigame_typer.timer = 0.0f;
    g_minigame_typer.cooldown = 0.0f;
}

static void minigame_typer_quit(void)
{
    free(g_minigame_typer.words);
}

static void minigame_typer_update(nkF32 dt)
{
    // Update the letter render angles at a fixed interval.
    g_minigame_typer.timer += dt;
    if(g_minigame_typer.timer >= 0.5f)
    {
        g_minigame_typer.timer -= 0.5f;
        for(nkS32 i=0,n=NK_ARRAY_SIZE(g_minigame_typer.angles); i<n; ++i)
        {
            nkF32 old_value = g_minigame_typer.angles[i];
            nkF32 new_value = g_minigame_typer.angles[i];
            while(fabsf(old_value-new_value) <= 0.15f || fabsf(old_value-new_value) >= 0.25f)
                new_value = rng_num_range(&g_rng_v, LETTER_MIN_ANGLE,LETTER_MAX_ANGLE);
            g_minigame_typer.angles[i] = new_value;
        }
    }

    // Cooldown if there was an incorrect letter given.
    if(g_minigame_typer.cooldown > 0.0f)
    {
        g_minigame_typer.cooldown -= dt;
    }

    if(game_is_playing() && g_minigame_typer.cooldown <= 0.0f)
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
                    sound_play(g_asset_sfx_office_stamp[rng_int_range(&g_rng_v, 0,4)], 0);

                    if(toupper(text_input[i]) == toupper(current_word[index]))
                    {
                        g_minigame_typer.input[index] = tolower(text_input[i]);
                    }
                    else
                    {
                        g_minigame_typer.cooldown = TYPING_COOLDOWN;
                        sound_play(g_asset_sfx_wrong_buzzer, 0);
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
}

static void minigame_typer_render(void)
{
    minigame_typer_draw_word(g_minigame_typer.current_word);

    // Draw the sad face if the player is on timeout.
    if(g_minigame_typer.cooldown > 0.0f)
    {
        nkF32 x = SCREEN_WIDTH * 0.5f;
        nkF32 y = SCREEN_HEIGHT * 0.5f;

        imm_begin_texture_batch(g_asset_ui);
        render_item_ex(x,y, 1,1, g_minigame_typer.angles[31], ATLAS_UI, ATLAS_UI_FEEDBACK_SAD_BODY, 1.0f);
        imm_end_texture_batch();
    }
}

/*////////////////////////////////////////////////////////////////////////////*/
