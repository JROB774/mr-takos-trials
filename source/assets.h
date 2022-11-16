#ifndef ASSETS_H__ /*/////////////////////////////////////////////////////////*/
#define ASSETS_H__

#include "codegen/atlas_particle.h"
#include "codegen/atlas_tako.h"
#include "codegen/atlas_ui.h"
#include "codegen/atlas_gametyper.h"
#include "codegen/atlas_gamesimon.h"

static Texture g_asset_back_paper;
static Texture g_asset_back_vignette;
static Texture g_asset_ui;
static Texture g_asset_particle;
static Texture g_asset_game_typer;
static Texture g_asset_game_simon;
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
static Sound   g_asset_sfx_mouse_click;
static Sound   g_asset_sfx_trumpet_fanfare;

static void load_all_assets(void);
static void free_all_assets(void);

static Sound    load_asset_sound  (const nkChar* name);
static Music    load_asset_music  (const nkChar* name);
static nkChar*  load_asset_text   (const nkChar* name);
static Font     load_asset_font   (const nkChar* name, nkF32 px_height);
static Texture  load_asset_texture(const nkChar* name, SamplerFilter filter, SamplerWrap wrap);
static Shader   load_asset_shader (const nkChar* name);

#endif /* ASSETS_H__ /////////////////////////////////////////////////////////*/
