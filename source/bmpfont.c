/*////////////////////////////////////////////////////////////////////////////*/

#define BITMAP_FONT_LINE_PADDING 20.0f

static nkF32 get_bitmap_font_width(const nkChar* text, nkF32 scale)
{
    nkF32 width = 0.0f;
    nkU32 text_length = strlen(text);
    for(nkU32 i=0; i<text_length; ++i)
    {
        nkS32 index = ATLAS_FONT_SOLID_A_SHADOW + (((toupper(text[i]) - 'A') * 2) + 1);
        width += ATLAS_FONT[index].clip_bounds.w * scale;
    }
    return width;
}

static nkF32 get_bitmap_font_pos_from_alignment(const nkChar* text, Alignment alignment, nkF32 scale)
{
    nkF32 w = get_bitmap_font_width(text, scale);
    switch(alignment)
    {
        case Alignment_Right: return (SCREEN_WIDTH - w); break;
        case Alignment_Center: return ((SCREEN_WIDTH - w) * 0.5f); break;
    }
    return 0.0f;
}

static void render_bitmap_font_aligned(const nkChar* text, Alignment alignment, nkF32 y, nkF32 scale, FontStyle style)
{
    render_bitmap_font(text, get_bitmap_font_pos_from_alignment(text, alignment, scale), y, scale, style);
}

static void render_bitmap_font(const nkChar* text, nkF32 x, nkF32 y, nkF32 scale, FontStyle style)
{
    nkU32 text_length = strlen(text);

    imm_begin_texture_batch(g_asset_font);
    for(nkU32 i=0; i<text_length; ++i)
    {
        nkS32 index = (NK_CHECK_FLAGS(style, FontStyle_Faded)) ? ATLAS_FONT_FADED_A_SHADOW : ATLAS_FONT_SOLID_A_SHADOW;
        index += (((toupper(text[i]) - 'A') * 2) + 1);

        nkF32 angle = 0.0f;
        if(NK_CHECK_FLAGS(style, FontStyle_Rotate))
        {
            angle = g_angles_big[i];
        }

        nkF32 ox = (NK_CHECK_FLAGS(style, FontStyle_Shake)) ? rng_int_range(-1,1) : 0.0f;
        nkF32 oy = (NK_CHECK_FLAGS(style, FontStyle_Shake)) ? rng_int_range(-1,1) : 0.0f;

        x += ((ATLAS_FONT[index].clip_bounds.w * 0.5f)) * scale;
        render_item_ex(x+ox,y+oy, scale,scale, angle, ATLAS_FONT, index, 1.0f);
        x += ((ATLAS_FONT[index].clip_bounds.w * 0.5f)) * scale;
    }
    imm_end_texture_batch();
}

static nkF32 bitmap_font_line_advance(nkF32 scale)
{
    return ((ATLAS_FONT[ATLAS_FONT_SOLID_A_BODY].clip_bounds.h + BITMAP_FONT_LINE_PADDING) * scale);
}

static nkF32 bitmap_font_block_height(nkS32 lines, nkF32 scale)
{
    return ((bitmap_font_line_advance(scale) * lines) - (BITMAP_FONT_LINE_PADDING * scale));
}

static nkF32 bitmap_font_block_y_off(nkS32 lines, nkF32 scale)
{
    nkF32 height = bitmap_font_block_height(lines, scale);
    nkF32 y = (SCREEN_HEIGHT - height) * 0.5f;
    return (y + ((ATLAS_FONT[ATLAS_FONT_SOLID_A_BODY].clip_bounds.h * scale) * 0.5f));
}

static ImmRect get_bitmap_font_bounds_aligned(const nkChar* text, Alignment alignment, nkF32 y, nkF32 scale, FontStyle style)
{
    return get_bitmap_font_bounds(text, get_bitmap_font_pos_from_alignment(text, alignment, scale), y, scale, style);
}

static ImmRect get_bitmap_font_bounds(const nkChar* text, nkF32 x, nkF32 y, nkF32 scale, FontStyle style)
{
    ImmRect bounds = NK_ZERO_MEM;
    bounds.x = x;
    bounds.y = y - ((ATLAS_FONT[ATLAS_FONT_SOLID_A_BODY].clip_bounds.h * scale) * 0.5f);
    bounds.w = get_bitmap_font_width(text, scale);
    bounds.h = (ATLAS_FONT[ATLAS_FONT_SOLID_A_BODY].clip_bounds.h * scale);
    return bounds;
}

/*////////////////////////////////////////////////////////////////////////////*/
