#ifndef GAME_H__ /*///////////////////////////////////////////////////////////*/
#define GAME_H__

NK_ENUM(MiniGameID, nkS32)
{
    MiniGameID_Typer,
    MiniGameID_Simon,
    MiniGameID_TOTAL
};

static void   game_init      (void);
static void   game_quit      (void);
static void   game_update    (nkF32 dt);
static void   game_render    (void);
static nkF32  game_get_timer (void);
static nkBool game_is_playing(void); // Used by mini-games to see if the timer is still going.

#endif /* GAME_H__ ///////////////////////////////////////////////////////////*/
