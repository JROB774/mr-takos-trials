#ifndef INPUT_H__ /*//////////////////////////////////////////////////////////*/
#define INPUT_H__

static void process_input_events(SDL_Event* event);
static void update_input_state(void);
static void reset_input_state(void);

// Keyboard
NK_ENUM(KeyCode, nkS32)
{
    KeyCode_Invalid,
    KeyCode_F1, KeyCode_F2, KeyCode_F3, KeyCode_F4, KeyCode_F5, KeyCode_F6,
    KeyCode_F7, KeyCode_F8, KeyCode_F9, KeyCode_F10, KeyCode_F11, KeyCode_F12,
    KeyCode_0, KeyCode_1, KeyCode_2, KeyCode_3, KeyCode_4,
    KeyCode_5, KeyCode_6, KeyCode_7, KeyCode_8, KeyCode_9,
    KeyCode_Q, KeyCode_W, KeyCode_E, KeyCode_R, KeyCode_T, KeyCode_Y, KeyCode_U,
    KeyCode_I, KeyCode_O, KeyCode_P, KeyCode_A, KeyCode_S, KeyCode_D, KeyCode_F,
    KeyCode_G, KeyCode_H, KeyCode_J, KeyCode_K, KeyCode_L, KeyCode_Z, KeyCode_X,
    KeyCode_C, KeyCode_V, KeyCode_B, KeyCode_N, KeyCode_M,
    KeyCode_Escape,
    KeyCode_Tab,
    KeyCode_CapsLock,
    KeyCode_Backspace,
    KeyCode_Enter,
    KeyCode_Space,
    KeyCode_Up, KeyCode_Right, KeyCode_Down, KeyCode_Left,
    KeyCode_LeftShift, KeyCode_RightShift,
    KeyCode_LeftCtrl, KeyCode_RightCtrl,
    KeyCode_LeftAlt, KeyCode_RightAlt,
    KeyCode_TOTAL
};

static nkBool is_key_down    (KeyCode code);
static nkBool is_key_up      (KeyCode code);
static nkBool is_key_pressed (KeyCode code);
static nkBool is_key_released(KeyCode code);

static nkBool is_any_key_down    (void);
static nkBool is_any_key_up      (void);
static nkBool is_any_key_pressed (void);
static nkBool is_any_key_released(void);

static nkChar* get_current_text_input(void);

// Mouse
NK_ENUM(MouseButton, nkS32)
{
    MouseButton_Invalid,
    MouseButton_Left,
    MouseButton_Middle,
    MouseButton_Right,
    MouseButton_TOTAL
};

static void   set_mouse_to_relative        (nkBool enable);
static nkBool is_mouse_relative            (void);
static void   set_mouse_position           (nkF32 x, nkF32 y);
static nkVec2 get_window_mouse_pos         (void);
static nkVec2 get_relative_mouse_pos       (void);
static nkVec2 get_screen_mouse_pos         (void);
static nkVec2 get_relative_screen_mouse_pos(void);

static nkBool is_mouse_button_down    (MouseButton button);
static nkBool is_mouse_button_up      (MouseButton button);
static nkBool is_mouse_button_pressed (MouseButton button);
static nkBool is_mouse_button_released(MouseButton button);

static nkBool is_any_mouse_button_down    (void);
static nkBool is_any_mouse_button_up      (void);
static nkBool is_any_mouse_button_pressed (void);
static nkBool is_any_mouse_button_released(void);

#endif /* INPUT_H__ //////////////////////////////////////////////////////////*/
