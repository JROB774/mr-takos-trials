#ifndef GAME_H__ /*///////////////////////////////////////////////////////////*/
#define GAME_H__

NK_ENUM(MiniGameID, nkS32)
{
    MiniGameID_Typer,
    MiniGameID_Simon,
    MiniGameID_Whack,
    MiniGameID_Match,
    MiniGameID_Catch,
    MiniGameID_TOTAL
};

static const nkChar* MINIGAME_TITLES[] =
{
    "TYPING TROUBLE",
    "BLOBO SING ALONG",
    "WHACK A GRUMBLE",
    "FIND THAT FACE",
    "APPLE PICKING",
    "RANDOM"
};

NK_STATIC_ASSERT(MiniGameID_TOTAL+1 == NK_ARRAY_SIZE(MINIGAME_TITLES), minigame_title_size_mismatch);

static void   game_init           (void);
static void   game_quit           (void);
static void   game_start          (MiniGameID minigame);
static void   game_end            (void);
static void   game_update         (nkF32 dt);
static void   game_render         (void);
static void   game_display_success(nkF32 x, nkF32 y);
static void   game_display_failure(nkF32 x, nkF32 y);
static nkF32  game_get_timer      (void);
static nkBool game_is_playing     (void); // Used by mini-games to see if the timer is still going.
static nkBool game_is_in_timeout  (void);
static nkBool game_is_in_countdown(void);

#endif /* GAME_H__ ///////////////////////////////////////////////////////////*/
