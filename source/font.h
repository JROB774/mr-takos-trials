#ifndef FONT_H__ /*///////////////////////////////////////////////////////////*/
#define FONT_H__

#define DECLARE_FONT_TYPE(name) struct name##__Type; typedef struct name##__Type* name;
#define DEFINE_FONT_TYPE(name) struct name##__Type

DECLARE_FONT_TYPE(Font);

static void   font_init           (void);
static void   font_quit           (void);
static Font   font_create         (void* data, nkBool owns_data, nkF32 px_height);
static void   font_destroy        (Font font);
static nkVec2 font_get_text_bounds(Font font, const nkChar* text);
static nkF32  font_get_text_width (Font font, const nkChar* text);
static nkF32  font_get_text_height(Font font, const nkChar* text);
static void   font_draw_text      (Font font, nkF32 x, nkF32 y, const nkChar* text, nkVec4 color);
static void   font_draw_char      (Font font, nkF32 x, nkF32 y, nkChar chr,         nkVec4 color);

#endif /* FONT_H__ ///////////////////////////////////////////////////////////*/
