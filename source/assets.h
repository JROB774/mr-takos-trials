#ifndef ASSETS_H__ /*/////////////////////////////////////////////////////////*/
#define ASSETS_H__

typedef struct TextureAtlasClip
{
    nkF32   original_width;
    nkF32   original_height;
    nkF32   offset_x;
    nkF32   offset_y;
    ImmRect clip_bounds;
}
TextureAtlasClip;

static Font    load_asset_font   (const nkChar* name, nkF32 px_height);
static Texture load_asset_texture(const nkChar* name, SamplerFilter filter, SamplerWrap wrap);
static Shader  load_asset_shader (const nkChar* name);

#endif /* ASSETS_H__ /////////////////////////////////////////////////////////*/
