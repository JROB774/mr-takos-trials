#ifndef PAUSE_H__ /*//////////////////////////////////////////////////////////*/
#define PAUSE_H__

static void   pause_update  (nkF32 dt);
static void   pause_render  (void);
static void   pause_game    (void);
static void   unpause_game  (voud);
static nkBool is_game_paused(void);

#endif /* PAUSE_H__ //////////////////////////////////////////////////////////*/
