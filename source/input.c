/*////////////////////////////////////////////////////////////////////////////*/

static const SDL_Scancode SCANCODE_MAP[KeyCode_TOTAL] =
{
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3, SDL_SCANCODE_F4, SDL_SCANCODE_F5, SDL_SCANCODE_F6,
    SDL_SCANCODE_F7, SDL_SCANCODE_F8, SDL_SCANCODE_F9, SDL_SCANCODE_F10, SDL_SCANCODE_F11, SDL_SCANCODE_F12,
    SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R, SDL_SCANCODE_T, SDL_SCANCODE_Y, SDL_SCANCODE_U,
    SDL_SCANCODE_I, SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_Z, SDL_SCANCODE_X,
    SDL_SCANCODE_C, SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N, SDL_SCANCODE_M,
    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_CAPSLOCK,
    SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_UP, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT,
    SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL,
    SDL_SCANCODE_LALT, SDL_SCANCODE_RALT
};

static const nkS32 BUTTON_MAP[MouseButton_TOTAL] =
{
    0, SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
};

typedef struct InputState
{
    nkBool prev_key_state[KeyCode_TOTAL];
    nkBool curr_key_state[KeyCode_TOTAL];
    nkBool prev_button_state[MouseButton_TOTAL];
    nkBool curr_button_state[MouseButton_TOTAL];
    nkVec2 mouse_pos;
    nkVec2 mouse_pos_relative;
    nkChar text_input[256];
}
InputState;

static InputState g_input;

static void process_input_events(SDL_Event* event)
{
    switch(event->type)
    {
        case SDL_TEXTINPUT:
        {
            nkU32 new_length = strlen(g_input.text_input) + strlen(event->text.text);
            if(new_length < NK_ARRAY_SIZE(g_input.text_input))
            {
                strcat(g_input.text_input, event->text.text);
            }
        } break;
    }
}

static void update_input_state(void)
{
    // Update the keyboard state.
    memcpy(g_input.prev_key_state, g_input.curr_key_state, sizeof(g_input.prev_key_state));
    const nkU8* sdl_keyboard_state = SDL_GetKeyboardState(NULL);
    for(nkS32 i=0; i<KeyCode_TOTAL; ++i)
        g_input.curr_key_state[i] = (sdl_keyboard_state[SCANCODE_MAP[NK_CAST(KeyCode, i)]] != 0);

    // Update the mouse state.
    nkS32 pmx,pmy;
    nkS32 rmx,rmy;

    SDL_GetMouseState(&pmx,&pmy);
    SDL_GetRelativeMouseState(&rmx,&rmy);

    g_input.mouse_pos         .x = NK_CAST(nkF32, pmx);
    g_input.mouse_pos         .y = NK_CAST(nkF32, pmy);
    g_input.mouse_pos_relative.x = NK_CAST(nkF32, rmx);
    g_input.mouse_pos_relative.y = NK_CAST(nkF32, rmy);

    // Update the mouse button state.
    nkU32 sdl_mouse = SDL_GetMouseState(NULL,NULL);
    memcpy(g_input.prev_button_state, g_input.curr_button_state, sizeof(g_input.prev_button_state));
    for(nkS32 i=0; i<MouseButton_TOTAL; ++i)
        g_input.curr_button_state[i] = (NK_CHECK_FLAGS(sdl_mouse, SDL_BUTTON(BUTTON_MAP[NK_CAST(MouseButton, i)])) != 0);
}

static void reset_input_state(void)
{
    memset(g_input.text_input, 0, sizeof(g_input.text_input));
}

static nkBool is_key_down(KeyCode code)
{
    if(code == KeyCode_Invalid) return NK_FALSE;
    return (g_input.curr_key_state[code] != 0);
}

static nkBool is_key_up(KeyCode code)
{
    if(code == KeyCode_Invalid) return NK_FALSE;
    return (g_input.curr_key_state[code] == 0);
}

static nkBool is_key_pressed(KeyCode code)
{
    if(code == KeyCode_Invalid) return NK_FALSE;
    return (g_input.curr_key_state[code] != 0 &&
            g_input.prev_key_state[code] == 0);
}

static nkBool is_key_released(KeyCode code)
{
    if(code == KeyCode_Invalid) return NK_FALSE;
    return (g_input.curr_key_state[code] == 0 &&
            g_input.prev_key_state[code] != 0);
}

static nkBool is_any_key_down(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, KeyCode_TOTAL); ++i)
        if(is_key_down(NK_CAST(KeyCode, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkBool is_any_key_up(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, KeyCode_TOTAL); ++i)
        if(is_key_up(NK_CAST(KeyCode, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkBool is_any_key_pressed(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, KeyCode_TOTAL); ++i)
        if(is_key_pressed(NK_CAST(KeyCode, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkBool is_any_key_released(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, KeyCode_TOTAL); ++i)
        if(is_key_released(NK_CAST(KeyCode, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkChar* get_current_text_input(void)
{
    return g_input.text_input;
}

static nkVec2 get_window_mouse_pos(void)
{
    return g_input.mouse_pos;
}

static nkVec2 get_relative_mouse_pos(void)
{
    return g_input.mouse_pos_relative;
}

static nkVec2 get_screen_mouse_pos(void)
{
    nkF32 ww = NK_CAST(nkF32, window_get_width());
    nkF32 wh = NK_CAST(nkF32, window_get_height());

    nkVec2 screen_mouse = get_window_mouse_pos();

    nkF32 scaledw = SCREEN_WIDTH;
    nkF32 scaledh = SCREEN_HEIGHT;

    while((scaledw+SCREEN_WIDTH <= ww) && (scaledh+SCREEN_HEIGHT <= wh))
    {
        scaledw += SCREEN_WIDTH;
        scaledh += SCREEN_HEIGHT;
    }

    nkF32 scaledx = (ww-scaledw)*0.5f;
    nkF32 scaledy = (wh-scaledh)*0.5f;

    nkF32 sx = ww / SCREEN_WIDTH;
    nkF32 sy = wh / SCREEN_HEIGHT;

    nkF32 s = nk_min(sx,sy);

    if(s < 1.0f) s = 1.0f; // Avoid scale of zero.

    screen_mouse.x = (screen_mouse.x - scaledx) / floorf(s);
    screen_mouse.y = (screen_mouse.y - scaledy) / floorf(s);

    return screen_mouse;
}

static nkBool is_mouse_button_down(MouseButton button)
{
    if(button == MouseButton_Invalid) return NK_FALSE;
    return (g_input.curr_button_state[button] != 0);
}

static nkBool is_mouse_button_up(MouseButton button)
{
    if(button == MouseButton_Invalid) return NK_FALSE;
    return (g_input.curr_button_state[button] == 0);
}

static nkBool is_mouse_button_pressed(MouseButton button)
{
    if(button == MouseButton_Invalid) return NK_FALSE;
    return (g_input.curr_button_state[button] != 0 &&
            g_input.prev_button_state[button] == 0);
}

static nkBool is_mouse_button_released(MouseButton button)
{
    if(button == MouseButton_Invalid) return NK_FALSE;
    return (g_input.curr_button_state[button] == 0 &&
            g_input.prev_button_state[button] != 0);
}

static nkBool is_any_mouse_button_down(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, MouseButton_TOTAL); ++i)
        if(is_mouse_button_down(NK_CAST(MouseButton, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkBool is_any_mouse_button_up(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, MouseButton_TOTAL); ++i)
        if(is_mouse_button_up(NK_CAST(MouseButton, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkBool is_any_mouse_button_pressed(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, MouseButton_TOTAL); ++i)
        if(is_mouse_button_pressed(NK_CAST(MouseButton, i))) return NK_TRUE;
    return NK_FALSE;
}

static nkBool is_any_mouse_button_released(void)
{
    for(nkS32 i=0; i<NK_CAST(nkS32, MouseButton_TOTAL); ++i)
        if(is_mouse_button_released(NK_CAST(MouseButton, i))) return NK_TRUE;
    return NK_FALSE;
}

/*////////////////////////////////////////////////////////////////////////////*/
