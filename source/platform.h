#ifndef PLATFORM_H__ /*///////////////////////////////////////////////////////*/
#define PLATFORM_H__

static const nkChar* get_base_path    (void);
static void          terminate        (void);
static void          fatal_error      (const nkChar* fmt, ...);
static void          user_error       (const nkChar* fmt, ...);
static nkS32         window_get_width (void);
static nkS32         window_get_height(void);
static void          set_fullscreen   (nkBool enable);
static nkBool        get_fullscreen   (void);
static void          show_cursor      (nkBool show);
static RenderTarget  get_screen       (void);

#endif /* PLATFORM_H__ ///////////////////////////////////////////////////////*/
