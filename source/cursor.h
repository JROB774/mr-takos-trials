#ifndef CURSOR_H__ /*/////////////////////////////////////////////////////////*/
#define CURSOR_H__

NK_ENUM(CursorType, nkS32)
{
    CursorType_Arrow,
    CursorType_Pointer,
    CursorType_Shovel,
    CursorType_Magnify,
    CursorType_TOTAL
};

static void    cursor_update    (nkF32 dt);
static void    cursor_render    (void);
static void    cursor_set_type  (CursorType type);
static nkBool  cursor_in_bounds (nkF32 x, nkF32 y, nkF32 w, nkF32 h);
static void    cursor_show      (void);
static void    cursor_hide      (void);
static ImmRect cursor_get_bounds(void);

#endif /* CURSOR_H__ /////////////////////////////////////////////////////////*/
