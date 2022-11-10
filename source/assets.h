#ifndef ASSETS_H__ /*/////////////////////////////////////////////////////////*/
#define ASSETS_H__

static Font    load_asset_font   (const nkChar* name, nkF32 px_height);
static Texture load_asset_texture(const nkChar* name, SamplerFilter filter, SamplerWrap wrap);
static Shader  load_asset_shader (const nkChar* name);

#endif /* ASSETS_H__ /////////////////////////////////////////////////////////*/
