#ifndef ASSETS_H__ /*/////////////////////////////////////////////////////////*/
#define ASSETS_H__

#include "codegen/atlas_letter.h"
#include "codegen/atlas_tako.h"
#include "codegen/atlas_ui.h"

static Texture g_asset_background;
static Texture g_asset_letter;
static Texture g_asset_ui;
static nkChar* g_asset_wordlist;
static Font    g_asset_font;
static Sound   g_asset_sfx_wrong_buzzer;
static Sound   g_asset_sfx_alarm_clock;
static Sound   g_asset_sfx_clock_ticking;
static Sound   g_asset_sfx_office_stamp[5];
static Sound   g_asset_sfx_crowd_gasp;
static Sound   g_asset_sfx_success_ding;
static Sound   g_asset_sfx_smack[3];
static Sound   g_asset_sfx_airhorn;

static void load_all_assets(void);
static void free_all_assets(void);

static Sound    load_asset_sound  (const nkChar* name);
static Music    load_asset_music  (const nkChar* name);
static nkChar*  load_asset_text   (const nkChar* name);
static Font     load_asset_font   (const nkChar* name, nkF32 px_height);
static Texture  load_asset_texture(const nkChar* name, SamplerFilter filter, SamplerWrap wrap);
static Shader   load_asset_shader (const nkChar* name);

#endif /* ASSETS_H__ /////////////////////////////////////////////////////////*/
