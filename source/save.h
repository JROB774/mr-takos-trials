#ifndef SAVE_H__ /*///////////////////////////////////////////////////////////*/
#define SAVE_H__

#define SAVE_FILE_FOURCC  NK_FOURCC('TAKO')
#define SAVE_FILE_VERSION 0

#pragma pack(push,1)
typedef struct SaveState
{
    nkU32 version;
    nkU32 fourcc;
    nkU32 highscore_typer;
}
SaveState;
#pragma pack(pop,1)

static SaveState g_save;

static void reset_game_data(void);
static void  save_game_data(void);
static void  load_game_data(void);

#endif /* SAVE_H__ ///////////////////////////////////////////////////////////*/
