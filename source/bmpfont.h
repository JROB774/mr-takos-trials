#ifndef BMP_FONT_H__ /*///////////////////////////////////////////////////////*/
#define BMP_FONT_H__

NK_ENUM(FontStyle, nkU32)
{
    FontStyle_None   =      0,
    FontStyle_Faded  = 1 << 0,
    FontStyle_Rotate = 1 << 1,
    FontStyle_Shake  = 1 << 2
};

NK_ENUM(Alignment, nkS32)
{
    Alignment_Left,
    Alignment_Right,
    Alignment_Center
};

static void    render_bitmap_font_aligned    (const nkChar* text, Alignment alignmment, nkF32 y, nkF32 scale, FontStyle style);
static void    render_bitmap_font            (const nkChar* text,              nkF32 x, nkF32 y, nkF32 scale, FontStyle style);
static nkF32   bitmap_font_line_advance      (nkF32 scale);
static nkF32   bitmap_font_block_height      (nkS32 lines, nkF32 scale);
static nkF32   bitmap_font_block_y_off       (nkS32 lines, nkF32 scale);
static ImmRect get_bitmap_font_bounds_aligned(const nkChar* text, Alignment alignmment, nkF32 y, nkF32 scale, FontStyle style);
static ImmRect get_bitmap_font_bounds        (const nkChar* text,              nkF32 x, nkF32 y, nkF32 scale, FontStyle style);

#endif /* BMPFONT_H__ ////////////////////////////////////////////////////////*/
