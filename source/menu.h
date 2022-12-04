#ifndef MENU_H__ /*///////////////////////////////////////////////////////////*/
#define MENU_H__

NK_ENUM(MenuState, nkS32)
{
    MenuState_Title,
    MenuState_Main,
    MenuState_Options,
    MenuState_Scores,
    MenuState_Credits,
    MenuState_Games,
    MenuState_TOTAL
};

static MenuState g_menustate;

static void menu_init  (void);
static void menu_quit  (void);
static void menu_update(nkF32 dt);
static void menu_render(void);

#endif /* MENU_H__ ///////////////////////////////////////////////////////////*/
