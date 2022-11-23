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

NK_ENUM(FontSize, nkS32)
{
    FontSize_Lil,
    FontSize_Big
};

static void    render_bitmap_font_aligned    (FontSize size, const nkChar* text, Alignment alignmment, nkF32 y, nkF32 scale, FontStyle style);
static void    render_bitmap_font            (FontSize size, const nkChar* text,              nkF32 x, nkF32 y, nkF32 scale, FontStyle style);
static nkF32   bitmap_font_line_advance      (FontSize size, nkF32 scale);
static nkF32   bitmap_font_block_height      (FontSize size, nkS32 lines, nkF32 scale);
static nkF32   bitmap_font_block_y_off       (FontSize size, nkS32 lines, nkF32 scale);
static ImmRect get_bitmap_font_bounds_aligned(FontSize size, const nkChar* text, Alignment alignmment, nkF32 y, nkF32 scale, FontStyle style);
static ImmRect get_bitmap_font_bounds        (FontSize size, const nkChar* text,              nkF32 x, nkF32 y, nkF32 scale, FontStyle style);

#endif /* BMPFONT_H__ ////////////////////////////////////////////////////////*/
