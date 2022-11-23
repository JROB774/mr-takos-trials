/*////////////////////////////////////////////////////////////////////////////*/

static const nkChar* TYPER_WORDLIST[] =
{
"apple", "apricot", "aptly", "accept", "admiral", "anvil", "badly", "bubble", "bomb", "boom", "buckle",
"backed", "batted", "baton", "blunder", "ball", "band", "beard", "bend", "bread", "crumble", "crab",
"cretin", "crazy", "crack", "corn", "clapped", "cold", "crib", "cram", "crumb", "clobber", "could",
"destroy", "deeply", "dumb", "daft", "dimly", "damp", "dime", "dread", "dribble", "dance", "daily",
"epic", "evil", "empire", "ember", "enter", "ended", "emblem", "ending", "envy", "ever", "electric",
"elect", "fight", "fire", "fright", "fate", "flabby", "flop", "found", "finder", "friend", "fiend",
"file", "fail", "flipped", "flip", "flipper", "ghost", "grumble", "grim", "grave", "grabbed", "grab",
"grown", "gown", "glob", "group", "gang", "grail", "gained", "grin", "goal", "higher", "height", "help",
"handy", "hinder", "hard", "harden", "herd", "heard", "hear", "held", "happy", "hoard", "hump", "hopped",
"hippo", "igloo", "idol", "idle", "island", "indigo", "ideal", "idea", "idiot", "imbecile", "isle",
"inlet", "inset", "iron", "ironed", "icon", "ignite", "inert", "jumping", "jump", "jailed", "joyous",
"joyful", "jumble", "jelly", "joined", "jargon", "jumper", "killer", "killed", "kite", "kiln", "kilt",
"kipper", "kitted", "knight", "knot", "knife", "knee", "lonely", "lobbed", "long", "leap", "lamped",
"leapt", "lame", "lingo", "lion", "lychee", "lending", "lied", "laid", "lead", "light", "late", "lane",
"lain", "leaned", "life", "left", "lady", "ladder", "manned", "mainly", "main", "monday", "maid", "madder",
"matey", "mate", "mighty", "morn", "maiden", "modal", "minor", "miner", "magpie", "master", "martian",
"morgue", "moaned", "martyr", "noble", "nylon", "night", "named", "name", "need", "notion", "neptune",
"noted", "never", "nailed", "nail", "orange", "option", "open", "oval", "opal", "ogle", "ooze", "oozing",
"openly", "onto", "omen", "oven", "opera", "organ", "orphan", "odor", "person", "preach", "painful",
"pain", "pander", "pending", "plan", "played", "playful", "paid", "payee", "payer", "pretend", "prior",
"phantom", "pardon", "poach", "plopped", "ponder", "power", "pray", "prey", "porch", "perch", "quit",
"quantum", "quiet", "quitter", "quaint", "quibble", "quart", "queen", "robot", "risen", "rise", "random",
"rainbow", "rely", "rained", "rain", "riot", "rondo", "react", "rate", "rein", "rind", "rend", "render",
"rhino", "runner", "ready", "real", "rail", "railway", "ripen", "show", "shown", "shore", "short", "shunt",
"shin", "shine", "shiny", "shamble", "sorrow", "slapper", "slap", "simple", "spam", "slump", "spot", "stop",
"stomp", "stolen", "style", "scam", "scum", "slim", "snap", "spool", "spun", "spin", "tramp", "trophy",
"triumph", "tripped", "trip", "trim", "taller", "tame", "team", "teller", "tied", "tanned", "timed",
"timer", "time", "tram", "tuesday", "tipper", "there", "their", "them", "then", "thin", "thimble", "thumb",
"thine", "trine", "tinned", "utopia", "usurp", "unify", "unity", "union", "undo", "upped", "upper",
"uranus", "ugly", "vicious", "victory", "vein", "vain", "veil", "vile", "vanity", "valiant", "window",
"winner", "wend", "wind", "winder", "willow", "warned", "wonky", "wand", "worded", "word", "wanted", "well",
};

typedef struct MiniGameTyper
{
    nkU32  current_word;
    nkChar input[32];
    nkS32  combo;
}
MiniGameTyper;

static MiniGameTyper g_minigame_typer;

static void minigame_typer_select_new_word(void)
{
    // Generate a new word and make sure it's not the same as the previous word.
    nkU32 new_word = g_minigame_typer.current_word;
    while(new_word == g_minigame_typer.current_word)
        new_word = rng_int() % NK_ARRAY_SIZE(TYPER_WORDLIST);
    g_minigame_typer.current_word = new_word;

    memset(g_minigame_typer.input, 0, sizeof(g_minigame_typer.input));
}

static void minigame_typer_init(void)
{
    // Nothing...
}

static void minigame_typer_quit(void)
{
    // Nothing...
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
        const nkChar* current_word = TYPER_WORDLIST[g_minigame_typer.current_word];
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
    static const nkF32 WORD_SPACING = 3.0f;

    const nkChar* word = TYPER_WORDLIST[g_minigame_typer.current_word];

    nkU32 word_length = strlen(word);

    nkF32 x = 0.0f;
    nkF32 y = 0.0f;
    nkF32 w = 0.0f;
    nkF32 h = 0.0f;

    // Calculate the bounds of the word so we can render it centered.
    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index = ATLAS_FONTBIG_SOLID_A_SHADOW + (((toupper(word[i]) - 'A') * 2) + 1);
        w += ATLAS_FONTBIG[index].clip_bounds.w + WORD_SPACING;
        h = nk_max(h, ATLAS_FONTBIG[index].clip_bounds.h);
    }

    x = (SCREEN_WIDTH - w) * 0.5f;
    y = SCREEN_HEIGHT * 0.5f;

    // Draw the current word.
    imm_begin_texture_batch(g_asset_font_big);
    for(nkU32 i=0; i<word_length; ++i)
    {
        nkS32 index = (g_minigame_typer.input[i] != word[i]) ? ATLAS_FONTBIG_FADED_A_SHADOW : ATLAS_FONTBIG_SOLID_A_SHADOW;

        index += (((toupper(word[i]) - 'A') * 2) + 1);

        nkF32 angle = g_angles_big[i];

        x += ((ATLAS_FONTBIG[index].clip_bounds.w * 0.5f));

        render_item_ex(x,y, 1,1, angle, ATLAS_FONTBIG, index, 1.0f);

        x += ((ATLAS_FONTBIG[index].clip_bounds.w * 0.5f));
        x += WORD_SPACING;
    }
    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
