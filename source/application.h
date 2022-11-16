#ifndef APPLICATION_H__ /*////////////////////////////////////////////////////*/
#define APPLICATION_H__

#define ITEM_ANGLE_CHANGE_SPEED 0.5f

NK_ENUM(AppState, nkS32)
{
    AppState_Menu,
    AppState_Game
};

static AppState g_appstate;

static const nkVec4 SHADOW_COLOR  = { 1.0f,1.0f,1.0f,0.6f };
static const nkVec4 BODY_COLOR    = { 1.0f,1.0f,1.0f,1.0f };
static const nkVec2 SHADOW_OFFSET = { 4.0f,4.0f };

static nkF32 update_item_angle(nkF32 old_angle, nkF32 min, nkF32 max);

static void render_item   (nkF32 x, nkF32 y,                                  const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 shadow_offset_multiplier);
static void render_item_ex(nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, const ImmAtlasClip* atlas_clips, nkU32 atlas_clip_index, nkF32 shadow_offset_multiplier);

static void app_init  (void);
static void app_quit  (void);
static void app_update(nkF32 dt);
static void app_render(void);

#endif /* APPLICATION_H__ ////////////////////////////////////////////////////*/
