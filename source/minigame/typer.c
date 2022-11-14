/*////////////////////////////////////////////////////////////////////////////*/

#define SUCCESS_COUNTDOWN  0.8f
#define WRONG_COUNTDOWN    0.8f
#define ANGLE_CHANGE_SPEED 0.5f
#define LETTER_MIN_ANGLE  -0.4f
#define LETTER_MAX_ANGLE   0.4f

typedef struct MiniGameTyper
{
    nkU32    current_word;
    nkChar** words;
    nkU32    word_count;
    nkChar   input[32];
    nkF32    angles[32];
    nkS32    score;
    nkS32    combo;
    nkF32    angle_timer;
    nkF32    success_countdown;
    nkF32    wrong_countdown;
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
        nkS32 index = ATLAS_LETTER_SOLID_A_SHADOW + (((toupper(word[i]) - 'A') * 2) + 1);
        w += ATLAS_LETTER[index].clip_bounds.w + WORD_SPACING;
        h = nk_max(h, ATLAS_LETTER[index].clip_bounds.h);
    }

    x = (SCREEN_WIDTH - w) * 0.5f;
    y = SCREEN_HEIGHT * 0.5f;

    imm_begin_texture_batch(g_asset_letter);

    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index = (g_minigame_typer.input[i] != word[i]) ? ATLAS_LETTER_FADED_A_SHADOW : ATLAS_LETTER_SOLID_A_SHADOW;;

        index += (((toupper(word[i]) - 'A') * 2) + 1);

        nkF32 angle = g_minigame_typer.angles[i];

        x += ((ATLAS_LETTER[index].clip_bounds.w * 0.5f));

        render_item_ex(x,y, 1,1, angle, ATLAS_LETTER, index, 1.0f);

        x += ((ATLAS_LETTER[index].clip_bounds.w * 0.5f));
        x += WORD_SPACING;
    }

    imm_end_texture_batch();
}

static void minigame_typer_draw_score(void)
{
    static const nkF32 LETTER_WIDTH = 15.0f;

    nkChar score_buffer[16] = NK_ZERO_MEM;
    sprintf(score_buffer, "%05d", g_minigame_typer.score);

    nkF32 x = (SCREEN_WIDTH - (LETTER_WIDTH*strlen(score_buffer))) * 0.5f;
    nkF32 y = SCREEN_HEIGHT - 32.0f;

    for(nkU32 i=0,n=strlen(score_buffer); i<n; ++i)
    {
        nkS32 index = ATLAS_UI_TIMER_0_SHADOW + (((score_buffer[i] - '0') * 2) + 1);
        x += LETTER_WIDTH * 0.5f;
        render_item_ex(x,y, 0.7f,0.7f, 0.0f, ATLAS_UI, index, 0.7f);
        x += LETTER_WIDTH * 0.5f;
    }
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

    // Setup the initial rendering angles;
    for(nkS32 i=0,n=NK_ARRAY_SIZE(g_minigame_typer.angles); i<n; ++i)
    {
        g_minigame_typer.angles[i] = rng_num_range(&g_rng_v, LETTER_MIN_ANGLE,LETTER_MAX_ANGLE);
    }
}

static void minigame_typer_quit(void)
{
    free(g_minigame_typer.words);
}

static void minigame_typer_start(void)
{
    minigame_typer_select_new_word();

    g_minigame_typer.score = 0;
    g_minigame_typer.combo = 0;

    g_minigame_typer.angle_timer = 0.0f;

    g_minigame_typer.success_countdown = 0.0f;
    g_minigame_typer.wrong_countdown = 0.0f;
}

static void minigame_typer_end(void)
{
    if(g_minigame_typer.score > g_save.highscore_typer)
    {
        g_save.highscore_typer = g_minigame_typer.score;
        save_game_data();
    }
}

static void minigame_typer_update(nkF32 dt)
{
    // Update the letter render angles at a fixed interval.
    g_minigame_typer.angle_timer += dt;
    if(g_minigame_typer.angle_timer >= ANGLE_CHANGE_SPEED)
    {
        g_minigame_typer.angle_timer -= ANGLE_CHANGE_SPEED;
        for(nkS32 i=0,n=NK_ARRAY_SIZE(g_minigame_typer.angles); i<n; ++i)
        {
            nkF32 old_value = g_minigame_typer.angles[i];
            nkF32 new_value = g_minigame_typer.angles[i];
            while(fabsf(old_value-new_value) <= 0.15f || fabsf(old_value-new_value) >= 0.25f)
                new_value = rng_num_range(&g_rng_v, LETTER_MIN_ANGLE,LETTER_MAX_ANGLE);
            g_minigame_typer.angles[i] = new_value;
        }
    }

    // Decrement the countdowns.
    if(g_minigame_typer.success_countdown > 0.0f)
    {
        g_minigame_typer.success_countdown -= dt;
    }
    if(g_minigame_typer.wrong_countdown > 0.0f)
    {
        g_minigame_typer.wrong_countdown -= dt;
    }

    if(game_is_playing() && g_minigame_typer.wrong_countdown <= 0.0f)
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

                        g_minigame_typer.combo++;
                        g_minigame_typer.score += 1 * g_minigame_typer.combo;
                    }
                    else
                    {
                        sound_play(g_asset_sfx_wrong_buzzer, 0);

                        g_minigame_typer.wrong_countdown = WRONG_COUNTDOWN;

                        g_minigame_typer.combo = 0;
                        g_minigame_typer.score -= 50;

                        if(g_minigame_typer.score < 0)
                        {
                            g_minigame_typer.score = 0;
                        }
                    }
                }
            }
        }

        // If the word is complete then advance!
        if(strcmp(g_minigame_typer.input, current_word) == 0)
        {
            g_minigame_typer.success_countdown = SUCCESS_COUNTDOWN;
            g_minigame_typer.score += 100; // Word bonus!
            minigame_typer_select_new_word();
            sound_play(g_asset_sfx_success_ding, 0);
        }
    }
}

static void minigame_typer_render(void)
{
    minigame_typer_draw_word(g_minigame_typer.current_word);

    imm_begin_texture_batch(g_asset_ui);

    minigame_typer_draw_score();

    // Draw the happy face if the player got a word.
    if(g_minigame_typer.success_countdown > 0.0f)
    {
        nkF32 x = (SCREEN_WIDTH * 0.5f) + 80.0f;
        nkF32 y = (SCREEN_HEIGHT - 32.0f);

        render_item_ex(x,y, 0.8f,0.8f, g_minigame_typer.angles[30], ATLAS_UI, ATLAS_UI_FEEDBACK_HAPPY_BODY, 1.0f);
    }

    // Draw the sad face if the player is on timeout.
    if(g_minigame_typer.wrong_countdown > 0.0f)
    {
        nkF32 x = (SCREEN_WIDTH * 0.5f);
        nkF32 y = (SCREEN_HEIGHT * 0.5f);

        render_item_ex(x,y, 1,1, g_minigame_typer.angles[31], ATLAS_UI, ATLAS_UI_FEEDBACK_SAD_BODY, 1.0f);
    }

    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
