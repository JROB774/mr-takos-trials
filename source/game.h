#ifndef GAME_H__ /*///////////////////////////////////////////////////////////*/
#define GAME_H__

static const nkVec4 SHADOW_COLOR  = { 1.0f,1.0f,1.0f,0.6f };
static const nkVec4 BODY_COLOR    = { 1.0f,1.0f,1.0f,1.0f };
static const nkVec2 SHADOW_OFFSET = { 4.0f,4.0f };

static RNG g_rng_l; // Logical - Used for generating logic related randomness.
static RNG g_rng_v; // Visual - Used for generating visual related randomness.

static void render_item   (nkF32 x, nkF32 y,                                  const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 alpha);
static void render_item_ex(nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 alpha);

static void   game_init      (void);
static void   game_quit      (void);
static void   game_update    (nkF32 dt);
static void   game_render    (void);
static nkBool game_is_playing(void); // Used by mini-games to see if the timer is still going.
static nkF32  game_get_timer (void);

#endif /* GAME_H__ ///////////////////////////////////////////////////////////*/
