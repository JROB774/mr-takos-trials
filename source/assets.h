#ifndef ASSETS_H__ /*/////////////////////////////////////////////////////////*/
#define ASSETS_H__

static Texture g_asset_background;
static Texture g_asset_letter;
static nkChar* g_asset_wordlist;

static void load_all_assets(void);
static void free_all_assets(void);

static nkChar*  load_asset_text   (const nkChar* name);
static Font     load_asset_font   (const nkChar* name, nkF32 px_height);
static Texture  load_asset_texture(const nkChar* name, SamplerFilter filter, SamplerWrap wrap);
static Shader   load_asset_shader (const nkChar* name);

#endif /* ASSETS_H__ /////////////////////////////////////////////////////////*/
