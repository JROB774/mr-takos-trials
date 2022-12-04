/*////////////////////////////////////////////////////////////////////////////*/

#define CATCH_BASKET_WIDTH  40.0f
#define CATCH_BASKET_HEIGHT 80.0f

#define CATCH_OBJECT_SIZE 32.0f
#define CATCH_OBJECT_VEL   5.5f

#define CATCH_MAX_OBJECT_COUNT 32

#define CATCH_MIN_SPAWN_TIME 0.2f
#define CATCH_MAX_SPAWN_TIME 0.9f

NK_ENUM(ObjectType, nkS32)
{
    ObjectType_Apple,
    ObjectType_Spiky,
    ObjectType_TOTAL
};

typedef struct Object
{
    ObjectType type;
    nkBool     active;
    nkVec2     pos;
    nkF32      vel;
    nkF32      angle;
    nkF32      scale;
}
Object;

typedef struct MiniGameCatch
{
    Object objects[CATCH_MAX_OBJECT_COUNT];
    nkF32  spawn_timer;
    nkVec2 basket_pos;
    nkBool basket_grabbed;
    nkS32  combo;
}
MiniGameCatch;

static MiniGameCatch g_minigame_catch;

static void minigame_catch_spawn_object(void)
{
    // Find an empty object slot.
    Object* o = NULL;
    for(nkS32 i=0; i<CATCH_MAX_OBJECT_COUNT; ++i)
    {
        if(!g_minigame_catch.objects[i].active)
        {
            o = &g_minigame_catch.objects[i];
            break;
        }
    }
    if(!o)
    {
        return; // No free slots!
    }

    // Setup the values.
    nkF32 min_x = (CATCH_OBJECT_SIZE * 2.5f);
    nkF32 max_x = SCREEN_WIDTH - (CATCH_OBJECT_SIZE * 2.5f);

    nkF32 rng = rng_num();

    o->type = ((rng <= 0.5f) ? ObjectType_Apple : ObjectType_Spiky);
    o->active = NK_TRUE;
    o->pos.x = rng_int_range(min_x,max_x);
    o->pos.y = -(CATCH_OBJECT_SIZE * 2.0f);
    o->vel = 0.0f;
    o->angle = rng_num_range(-0.3f,0.3f);
    o->scale = rng_num_range(0.8f,1.0f);

    // Play the sound effects.
    sound_play(g_asset_sfx_twig_snap[rng_int_range(0,3)], 0);
    sound_play(g_asset_sfx_leaves_rustle, 0);
}

static nkBool minigame_catch_check_collision(Object* o, nkF32 bx, nkF32 by, nkF32 bw, nkF32 bh)
{
    nkF32 x = o->pos.x - (CATCH_OBJECT_SIZE * 0.5f);
    nkF32 y = o->pos.y - (CATCH_OBJECT_SIZE * 0.5f);
    nkF32 w = CATCH_OBJECT_SIZE;
    nkF32 h = CATCH_OBJECT_SIZE;

    return ((bx < (x+w)) && ((bx+bh) > x) && (by < (y+h)) && ((by+bh) > y));
}

static void minigame_catch_grab_basket(void)
{
    g_minigame_catch.basket_grabbed = NK_TRUE;
    cursor_hide();
}

static void minigame_catch_release_basket(void)
{
    g_minigame_catch.basket_grabbed = NK_FALSE;
    cursor_show();
}

static void minigame_catch_start(void)
{
    cursor_set_type(CursorType_Grabber);

    memset(g_minigame_catch.objects, 0, sizeof(g_minigame_catch.objects));

    g_minigame_catch.spawn_timer = 0.5f;

    g_minigame_catch.basket_pos.x = SCREEN_WIDTH * 0.5f;
    g_minigame_catch.basket_pos.y = SCREEN_HEIGHT - 72.0f;

    g_minigame_catch.basket_grabbed = NK_FALSE;

    g_minigame_catch.combo = 0;
}

static void minigame_catch_end(void)
{
    // Nothing...
}

static void minigame_catch_update(nkF32 dt)
{
    if(game_is_playing())
    {
        // Handle the player grabbing/releasing/moving the basket.
        if(!g_minigame_catch.basket_grabbed)
        {
            // Grab the basket if the cursor collides.
            if(is_mouse_button_pressed(MouseButton_Left))
            {
                nkF32 bx = g_minigame_catch.basket_pos.x - (CATCH_BASKET_WIDTH * 0.5f);
                nkF32 by = g_minigame_catch.basket_pos.y - (CATCH_BASKET_HEIGHT * 0.5f);
                nkF32 bw = CATCH_BASKET_WIDTH;
                nkF32 bh = CATCH_BASKET_HEIGHT;

                if(cursor_in_bounds(bx,by,bw,bh))
                {
                    minigame_catch_grab_basket();
                }
            }
        }
        else
        {
            // Make the basket follow the mouse horizontally.
            g_minigame_catch.basket_pos.x = get_screen_mouse_pos().x;
            g_minigame_catch.basket_pos.x = nk_clamp(g_minigame_catch.basket_pos.x, 0.0f, SCREEN_WIDTH);

            // Release the basket.
            if(is_mouse_button_released(MouseButton_Left))
            {
                minigame_catch_release_basket();
            }
        }

        // Spawn new falling objects.
        g_minigame_catch.spawn_timer -= dt;
        if(g_minigame_catch.spawn_timer <= 0.0f)
        {
            nkF32 min_time = CATCH_MIN_SPAWN_TIME;
            nkF32 max_time = CATCH_MAX_SPAWN_TIME;

            nkF32 timer = game_get_timer();
            if(timer <= 5.0f) max_time -= 0.3f;
            else if(timer <= 10.0f) max_time -= 0.15f;

            g_minigame_catch.spawn_timer = rng_num_range(min_time,max_time);
            minigame_catch_spawn_object();
        }
    }
    else
    {
        if(g_minigame_catch.basket_grabbed)
        {
            minigame_catch_release_basket();
        }
    }

    // Update objects and check collisions with the basket.
    nkF32 bx = g_minigame_catch.basket_pos.x - (CATCH_BASKET_WIDTH * 0.5f);
    nkF32 by = g_minigame_catch.basket_pos.y - (CATCH_BASKET_HEIGHT * 0.5f);
    nkF32 bw = CATCH_BASKET_WIDTH;
    nkF32 bh = CATCH_BASKET_HEIGHT;

    by += 20.0f; // Collider adjustment to feel nice.
    bh -= 20.0f;

    for(nkS32 i=0; i<CATCH_MAX_OBJECT_COUNT; ++i)
    {
        Object* o = &g_minigame_catch.objects[i];
        if(o->active)
        {
            o->vel += CATCH_OBJECT_VEL;
            if(o->type == ObjectType_Spiky)
                o->vel += 0.5f;
            o->pos.y += o->vel * dt;

            if(o->pos.y > (SCREEN_HEIGHT + (CATCH_OBJECT_SIZE * 2.0f)))
            {
                o->active = NK_FALSE;
            }

            if(game_is_playing() && !game_is_in_timeout())
            {
                if(minigame_catch_check_collision(o, bx,by,bw,bh) && (o->pos.y < by))
                {
                    o->active = NK_FALSE;

                    if(o->type == ObjectType_Apple)
                    {
                        game_display_success((SCREEN_WIDTH * 0.5f) + 80.0f, (SCREEN_HEIGHT - 32.0f));
                        g_gamestate.game_score += 100 + (10 * g_minigame_catch.combo);
                        g_minigame_catch.combo++;
                    }
                    if(o->type == ObjectType_Spiky)
                    {
                        game_display_failure((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));
                        sound_play(g_asset_sfx_smack[rng_int_range(0,2)], 0);
                        g_minigame_catch.combo = 0;
                        g_gamestate.game_score -= 200;
                    }
                }
            }
        }
    }
}

static void minigame_catch_render(void)
{
    imm_begin_texture_batch(g_asset_catch);

    // Draw the objects.
    for(nkS32 i=0; i<CATCH_MAX_OBJECT_COUNT; ++i)
    {
        Object* o = &g_minigame_catch.objects[i];
        if(o->active)
        {
            nkS32 index = (o->type == ObjectType_Apple) ? ATLAS_CATCH_APPLE_BODY : ATLAS_CATCH_SPIKY_BODY;
            render_item_ex(o->pos.x,o->pos.y, o->scale,o->scale, o->angle+g_angles_lil[i], ATLAS_CATCH, index, 1.0f);
        }
    }

    // Draw the basket.
    nkF32 bx = g_minigame_catch.basket_pos.x;
    nkF32 by = g_minigame_catch.basket_pos.y;
    nkF32 angle = (g_minigame_catch.basket_grabbed) ? g_angles_lil[APP_MAX_ANGLES-1] : 0.0f;
    render_item_ex(bx,by, 1,1, angle, ATLAS_CATCH, ATLAS_CATCH_BASKET_BODY, 1.0f);

    imm_end_texture_batch();
}

/*////////////////////////////////////////////////////////////////////////////*/
