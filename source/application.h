#ifndef APPLICATION_H__ /*////////////////////////////////////////////////////*/
#define APPLICATION_H__

NK_ENUM(AppState, nkS32)
{
    AppState_Menu,
    AppState_Game
};

static AppState g_appstate;

static const nkVec4 SHADOW_COLOR  = { 1.0f,1.0f,1.0f,0.6f };
static const nkVec4 BODY_COLOR    = { 1.0f,1.0f,1.0f,1.0f };
static const nkVec2 SHADOW_OFFSET = { 4.0f,4.0f };

static RNG g_rng_l; // Logical - Used for generating logic related randomness.
static RNG g_rng_v; // Visual - Used for generating visual related randomness.

static void render_item   (nkF32 x, nkF32 y,                                  const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index);
static void render_item_ex(nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index);

static void app_init  (void);
static void app_quit  (void);
static void app_update(nkF32 dt);
static void app_render(void);

#endif /* APPLICATION_H__ ////////////////////////////////////////////////////*/
