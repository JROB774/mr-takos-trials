#ifndef CURSOR_H__ /*/////////////////////////////////////////////////////////*/
#define CURSOR_H__

static void   cursor_update(nkF32 dt);
static void   cursor_render(void);
static nkBool cursor_in_bounds(nkF32 x, nkF32 y, nkF32 w, nkF32 h);
static void   cursor_show(void);
static void   cursor_hide(void);

#endif /* CURSOR_H__ /////////////////////////////////////////////////////////*/
