/*////////////////////////////////////////////////////////////////////////////*/

#define MATCH_FACE_SCALE 0.7f

#define MATCH_BOARD_MIN_X (96)
#define MATCH_BOARD_MIN_Y (96)
#define MATCH_BOARD_MAX_X (SCREEN_WIDTH-96)
#define MATCH_BOARD_MAX_Y (SCREEN_HEIGHT-96)

NK_ENUM(MatchFaceID, nkS32)
{
    MatchFaceID_Tache,
    MatchFaceID_Skull,
    MatchFaceID_Happy,
    MatchFaceID_Glass,
    MatchFaceID_Shock,
    MatchFaceID_Saddo,
    MatchFaceID_TOTAL
};

typedef struct MatchFace
{
    nkF32       x,y;
    MatchFaceID type;
}
MatchFace;

typedef struct MiniGameMatch
{
    MatchFaceID face_to_find;
    MatchFace   faces[128];
    nkU32       face_total;
    nkS32       stage;
    nkS32       combo;
}
MiniGameMatch;

static MiniGameMatch g_minigame_match;

static void minigame_match_pick_new_face(void)
{
    // Pick a new face to find.
    MatchFaceID old_face = g_minigame_match.face_to_find;
    while(old_face == g_minigame_match.face_to_find)
    {
        g_minigame_match.face_to_find = rng_int_range(0,MatchFaceID_TOTAL-1);
    }

    // Generate board of faces.
    g_minigame_match.face_total = 5; // @Incomplete: Scale with the current stage...
    for(nkU32 i=0; i<g_minigame_match.face_total; ++i)
    {
        MatchFace* f = &g_minigame_match.faces[i];
        f->x = rng_int_range(MATCH_BOARD_MIN_X, MATCH_BOARD_MAX_X);
        f->y = rng_int_range(MATCH_BOARD_MIN_Y, MATCH_BOARD_MAX_Y);
        f->type = g_minigame_match.face_to_find;
        while(f->type == g_minigame_match.face_to_find)
        {
            f->type = rng_int_range(0,MatchFaceID_TOTAL-1);
        }
    }

    // Pick a random face to be the one to find.
    nkU32 index = rng_int_range(0, g_minigame_match.face_total-1);
    g_minigame_match.faces[index].type = g_minigame_match.face_to_find;
}

static void minigame_match_start(void)
{
    cursor_set_type(CursorType_Magnify);

    g_minigame_match.stage = 0;
    g_minigame_match.combo = 0;

    minigame_match_pick_new_face();
}

static void minigame_match_end(void)
{
    // Nothing...
}

static void minigame_match_update(nkF32 dt)
{
    if(game_is_playing())
    {
        // Check if the user has clicked on the correct face or not.
        if(is_mouse_button_pressed(MouseButton_Left))
        {
            nkBool found_the_face = NK_FALSE;

            for(nkU32 i=0; i<g_minigame_match.face_total; ++i)
            {
                MatchFace* f = &g_minigame_match.faces[i];

                nkF32 x = f->x - 20.0f;
                nkF32 y = f->y - 20.0f;
                nkF32 w = 40.0f;
                nkF32 h = 40.0f;

                if(f->type == g_minigame_match.face_to_find && cursor_in_bounds(x,y,w,h))
                {
                    found_the_face = NK_TRUE;
                    break;
                }
            }

            if(found_the_face)
            {
                game_display_success((SCREEN_WIDTH * 0.5f) + 80.0f, (SCREEN_HEIGHT - 32.0f));
            }
            else
            {
                game_display_failure((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));
            }

            minigame_match_pick_new_face();
        }
    }
}

static void minigame_match_render(void)
{
    imm_begin_texture_batch(g_asset_match);

    // Draw all the faces on the board.
    for(nkU32 i=0; i<g_minigame_match.face_total; ++i)
    {
        MatchFace* f = &g_minigame_match.faces[i];
        nkS32 index = ATLAS_MATCH_0_SHADOW + ((f->type * 2) + 1);
        render_item_ex(f->x,f->y, MATCH_FACE_SCALE,MATCH_FACE_SCALE, 0.0f, ATLAS_MATCH, index, 1.0f);
    }

    // Draw the face to find in the top-left corner.
    nkS32 index = ATLAS_MATCH_0_SHADOW + ((g_minigame_match.face_to_find * 2) + 1);
    nkF32 x = 28.0f;
    nkF32 y = 32.0f;
    render_item_ex(x,y, MATCH_FACE_SCALE,MATCH_FACE_SCALE, 0.0f, ATLAS_MATCH, index, 1.0f);

    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
