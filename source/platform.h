#ifndef PLATFORM_H__ /*///////////////////////////////////////////////////////*/
#define PLATFORM_H__

static void   fatal_error      (const nkChar* fmt, ...);
static nkS32  window_get_width (void);
static nkS32  window_get_height(void);
static void   set_fullscreen   (nkBool enable);
static nkBool get_fullscreen   (void);

#endif /* PLATFORM_H__ ///////////////////////////////////////////////////////*/
