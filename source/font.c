/*////////////////////////////////////////////////////////////////////////////*/

#define ALLOCATE_FONT_TYPE(name) malloc(sizeof(struct name##__Type))

#define FONT_ATLAS_SIZE 512

DEFINE_FONT_TYPE(Font)
{
    Texture          atlas;
    stbtt_bakedchar* chars;
    nkU8*            data_buffer;
    nkBool           owns_data;
    nkF32            px_height;
};

static Font font_create(void* data, nkBool owns_data, nkF32 px_height)
{
    Font font = ALLOCATE_FONT_TYPE(Font);
    if(!font)
        fatal_error("Failed to allocate font!");

    font->data_buffer = data;
    font->owns_data = owns_data;

    font->chars = malloc(96 * sizeof(stbtt_bakedchar));
    if(!font->chars)
        fatal_error("Failed to allocate chars for font!");

    nkU8* pixels = malloc(FONT_ATLAS_SIZE*FONT_ATLAS_SIZE);
    if(!pixels)
        fatal_error("Failed to allocate pixels for font!");

    stbtt_BakeFontBitmap(font->data_buffer, 0, px_height, pixels, FONT_ATLAS_SIZE,FONT_ATLAS_SIZE, 32,96, font->chars);

    font->atlas = texture_create(FONT_ATLAS_SIZE,FONT_ATLAS_SIZE, 1, pixels, SamplerFilter_Linear, SamplerWrap_Clamp);
    font->px_height = px_height;

    free(pixels);

    return font;
}

static void font_destroy(Font font)
{
    if(!font) return;
    texture_destroy(font->atlas);
    if(font->owns_data)
        free(font->data_buffer);
    free(font);
}

static stbtt_aligned_quad get_font_glyph_quad(Font font, nkChar c, nkF32* x, nkF32* y)
{
    NK_ASSERT(font);

    NK_ASSERT(x);
    NK_ASSERT(y);

    nkF32 w = texture_get_width(font->atlas);
    nkF32 h = texture_get_height(font->atlas);

    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(font->chars, w,h, c-32, x,y,&q, 1);

    return q;
}

static nkVec2 font_get_text_bounds(Font font, const nkChar* text)
{
    NK_ASSERT(font);

    nkVec2 bounds = NK_ZERO_MEM;
    for(nkU64 i=0,n=strlen(text); i<n; ++i)
    {
        nkChar c = text[i];
        if(c >= 32 && c < 128)
            font_get_glyph_quad(font, c, &bounds.x,&bounds.y);
    }
    return bounds;
}

static nkF32 font_get_text_width(Font font, const nkChar* text)
{
    return font_get_text_bounds(font, text).x;
}

static nkF32 font_get_text_height(Font font, const nkChar* text)
{
    return font_get_text_bounds(font, text).y;
}

static void font_draw_text(Font font, nkF32 x, nkF32 y, const nkChar* text, nkVec4 color)
{
    NK_ASSERT(font);
    NK_ASSERT(text);

    nkF32 start_x = x;
    nkF32 start_y = y;

    if(strlen(text) == 0)
    {
        return;
    }

    imm_begin(DrawMode_Triangles, font->atlas);
    while(*text)
    {
        if(*text >= 32 && *text < 128)
        {
            stbtt_aligned_quad q = get_font_glyph_quad(font, *text, &x,&y);

            imm_vertex((ImmVertex){ (nkVec2){ q.x0,q.y1 }, (nkVec2){ q.s0,q.t1 }, color }); // BL
            imm_vertex((ImmVertex){ (nkVec2){ q.x0,q.y0 }, (nkVec2){ q.s0,q.t0 }, color }); // TL
            imm_vertex((ImmVertex){ (nkVec2){ q.x1,q.y0 }, (nkVec2){ q.s1,q.t0 }, color }); // TR
            imm_vertex((ImmVertex){ (nkVec2){ q.x1,q.y0 }, (nkVec2){ q.s1,q.t0 }, color }); // TR
            imm_vertex((ImmVertex){ (nkVec2){ q.x1,q.y1 }, (nkVec2){ q.s1,q.t1 }, color }); // BR
            imm_vertex((ImmVertex){ (nkVec2){ q.x0,q.y1 }, (nkVec2){ q.s0,q.t1 }, color }); // BL
        }
        else if(*text == '\n')
        {
            x = start_x;
            y += font->px_height;
        }
        ++text;
    }
    imm_end();
}

static void font_draw_char(Font font, nkF32 x, nkF32 y, nkChar chr, nkVec4 color)
{
    nkChar buffer[2] = { chr, '\0' };
    font_draw_text(font, x,y, buffer, color);
}

/*////////////////////////////////////////////////////////////////////////////*/
