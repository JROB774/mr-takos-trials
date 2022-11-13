/*////////////////////////////////////////////////////////////////////////////*/

typedef struct MiniGameTyper
{
    nkChar** words;
    nkU32    word_count;
}
MiniGameTyper;

static MiniGameTyper g_minigame_typer;

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

static void minigame_typer_update(nkF32 dt)
{
    // @Incomplete: ...
}

static void minigame_typer_render(void)
{
    // @Incomplete: ...
}

/*////////////////////////////////////////////////////////////////////////////*/
