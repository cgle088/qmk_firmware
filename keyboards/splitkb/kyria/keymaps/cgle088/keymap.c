/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "split_util.h"

enum layers {
    _DVORAK = 0,
    _QWERTY,
    _COLEMAK_DH,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
    TD_LSHIFT_PAREN,
    TD_RSHIFT_CURLY
};

enum{
    SAVE,
    COPY,
    NEW,
    NEW_TAB,
    LSHIFT,
    FIND_PAGE,
    OBJ_ARROW
};

typedef enum {
	TD_NONE,
	TD_SINGLE_TAP,
	TD_SINGLE_HOLD,
	TD_DOUBLE_TAP,
	TD_TRIPLE_TAP,
    TD_UNKNOWN,
} td_state_t;

typedef struct {
	bool is_press_action;
	td_state_t state;
} td_tap_t;

td_state_t cur_dance(qk_tap_dance_state_t *state);

void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);

bool is_alt_tab_active = false; // ADD this near the begining of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.
enum custom_keycodes {          // Make sure have the awesome keycode ready
    ALT_TAB = SAFE_RANGE,
    SAVE_TAB,
    CTRL_PGUP,
    CTRL_PGDN
};

// #ifdef TAP_DANCE_ENABLE
// qk_tap_dance_action_t tap_dance_actions[] = {
//   //[TD_LSHIFT_PAREN] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT_PAREN, KC_LSFT),
//   [TD_LSHIFT_PAREN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, SHIFT_FINISHED, SHIFT_RESET),
//   [TD_RSHIFT_CURLY] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT_CURLY_BRACE, KC_RSFT),
// //   [] = ACTION_TAP_DANCE_DOUBLE(,),
// };
// #endif

#ifdef RGBLIGHT_ENABLE
const rgblight_segment_t PROGMEM default_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {
    1, 36, HSV_PINK
  }
);
const rgblight_segment_t PROGMEM sym_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {
    1, 12, HSV_CYAN
  }
);

const rgblight_segment_t PROGMEM nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {
    1, 24, HSV_PURPLE
  }
);

const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
  default_layer,
  sym_layer,
  nav_layer
);

void keyboard_post_init_user(void){
  rgblight_layers = rgb_layers;
}
#endif

// Aliases for readability
#define QWERTY   DF(_QWERTY)
#define COLEMAK  DF(_COLEMAK_DH)
#define DVORAK   DF(_DVORAK)

#define SYM      MO(_SYM)
#define NAV      MO(_NAV)
#define FKEYS    MO(_FUNCTION)
#define ADJUST   MO(_ADJUST)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)
#define SHFT_PAREN SFT_T(KC_LEFT_PAREN)

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: Dvorak
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   | ' "  | , <  | . >  |   P  |   Y  |                              |   F  |   G  |   C  |   R  |   L  | ALT_TAB|
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   O  |   E  |   U  |   I  |                              |   D  |   H  |   T  |   N  |   S  |Enter   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift | ; :  |   Q  |   J  |   K  |   X  | [ {  |CapsLk|  |F-keys|  ] } |   B  |   M  |   W  |   V  |   Z  | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust|   ←  |  →   | Space| Sym  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *            ueu            `----------------------------------'  `----------------------------------'
 */
    [_DVORAK] = LAYOUT(
     KC_TAB,KC_QUOTE,KC_COMM,  KC_DOT,   TD(KC_P) ,   KC_Y ,                                        KC_F,   KC_G ,  TD(COPY) ,  KC_R ,  KC_L , ALT_TAB,
     CTL_ESC, KC_A,  KC_O,  KC_E,   KC_U ,   KC_I,                                       KC_D,   KC_H , KC_T,  TD(NEW),  TD(SAVE) , KC_ENT,
     TD(LSHIFT), KC_SCLN, KC_Q   ,  KC_J  ,   KC_K ,   KC_X , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_B,   KC_M ,  KC_W,   KC_V ,  KC_Z, KC_LEFT_CURLY_BRACE,
                                 MO(_SYM), KC_LEFT, KC_RGHT, KC_SPC , MO(_SYM),     TO(_SYM), KC_BSPC ,KC_UP, KC_DOWN, KC_APP
    ),
/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    `   |  1   |  2   |  <   |  >   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    ~   |  /   |  =   |  !   |  $   |  _   |                              |   ^  |  4   |  5   |  6   |  )   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |SAVE_TAB|   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  1   |  2   |  3   |  /   |   ?    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |  _   |  .   |      |  |  Nav |      |  0   |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
      KC_GRV ,   KC_1 ,   KC_2 ,   KC_LT ,   TD(OBJ_ARROW),   KC_5 ,                                        KC_AMPR,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 , KC_EQL ,
     KC_TILD , KC_SLSH,  KC_EQL, KC_EXLM,  KC_DLR, KC_UNDS,                                     KC_CIRC, KC_4, KC_5, KC_6, KC_RPRN, KC_PLUS,
     SAVE_TAB , KC_BSLS, KC_COLN, KC_SCLN, KC_MINS, KC_LBRC, KC_LCBR, _______, _______, KC_RCBR, KC_RBRC, KC_1, KC_2,  KC_3, KC_SLSH, KC_QUES,
                                 _______, RCS(KC_LEFT), RCS(KC_RIGHT), _______, _______, TO(_NAV), _______, KC_0 , _______, _______
    ),
/*ueouoeu
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_VOLU, KC_DEL,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_VOLD, KC_INS,
      _______, _______, _______, _______, _______, _______, _______, KC_SLCK, _______, _______,KC_PAUSE, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_PSCR,
                                 _______, _______, _______, _______, _______, TO(_DVORAK), _______, _______, _______, _______
    ),


/*
 * Function Layer: Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  | F10  | F11  | F12  |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |                              |      | Shift| Ctrl |  Alt |  GUI |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCTION] = LAYOUT(
      _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                                     _______, _______, _______, _______, _______, _______,
      _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                                     _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
      _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |QWERTY|      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |Dvorak|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, _______, _______, QWERTY , _______, _______,                                    _______, _______, _______, _______,  _______, _______,
      _______, _______, _______, DVORAK , _______, _______,                                    RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI,  RGB_MOD, _______,
      _______, _______, _______, COLEMAK, _______, _______,_______, _______, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______,
                                 _______, _______, _______,_______, _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */
      /* void render_logo(void){

       // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }*/
#ifdef OLED_ENABLE
void oled_task_user(void){
    oled_write_P(PSTR("Layer:"), false);

    switch (get_highest_layer(layer_state)){
        case _DVORAK:
            oled_write_P(PSTR("Default\n")), false);
            break;
        case _SYM:
            oled_write_P(PSTR("SYM\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
        led_t led_state = host_keyboard_led_state();
        oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
        oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
        oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif

/*layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _RAISE:
        rgblight_setrgb (0x00,  0x00, 0xFF);
        break;
    case _LOWER:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case _PLOVER:
        rgblight_setrgb (0x00,  0xFF, 0x00);
        break;
    case _ADJUST:
        rgblight_setrgb (0x7A,  0x00, 0xFF);
        break;
    default: //  for any other layers, or the default layer
        rgblight_setrgb (0x00,  0xFF, 0xFF);
        break;
    }
  return state;
}*/
#ifdef RGBLIGHT_ENABLE
layer_state_t default_layer_state_set_user(layer_state_t state){
	rgblight_set_layer_state(0, layer_state_cmp(state, _DVORAK));
	return state;
}

layer_state_t layer_state_set_user(layer_state_t state){
  rgblight_set_layer_state(1, layer_state_cmp(state, _SYM));
  rgblight_set_layer_state(2, layer_state_cmp(state, _NAV));
  return state;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // DEFAULT Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    //RIGHT ENCODER
    if (index == 1) {
      if(IS_LAYER_ON(_SYM)){
        if(clockwise){
          //select rightwards
          register_code(KC_RCTL);
          tap_code(KC_PGDN);
        }else{
          register_code(KC_RCTL);
          tap_code(KC_PGUP);
          //select leftwards
        }
        unregister_code(KC_RCTL);
          //on click select word
      }else{
        // DEFAULT Page up/Page down
        if (clockwise) {
          if (is_alt_tab_active) {
            tap_code(KC_RIGHT);
            alt_tab_timer = timer_read();
          } else {
            //DEFAULT ENCODER
            tap_code(KC_PGDN);
          }
        } else {
          if (is_alt_tab_active) {
            alt_tab_timer = timer_read();
            tap_code(KC_LEFT);
          } else {
            //DEFAULT ENCODER
            tap_code(KC_PGUP);
          }
        }
      }
    }
/*    if (index == 2) {
        // DEFAULT Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }*/

    return false;
}
#endif

uint8_t mod_state;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  mod_state = get_mods();
  switch (keycode) { // This will do most of the grunt work with the keycodes.
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
          alt_tab_timer = timer_read();
          register_code(KC_TAB);
        }else{
          tap_code(KC_ENTER);
        }
      } else {
        unregister_code(KC_TAB);
      }
      break;
    case SAVE_TAB:
        if (record->event.pressed) {
            register_code(KC_RCTL);
            tap_code(KC_S);
            unregister_code(KC_RCTL);
            register_code(KC_LALT);
            register_code(KC_TAB);
            unregister_code(KC_TAB);
            unregister_code(KC_LALT);
            wait_ms(200);
            register_code(KC_RCTL);
            tap_code(KC_R);
            unregister_code(KC_RCTL);
        }
        break;
    case KC_BSPC:{
        //https://beta.docs.qmk.fm/using-qmk/simple-keycodes/feature_advanced_keycode
        bool delkey_registered;
        if(record->event.pressed){
          if(mod_state & MOD_MASK_SHIFT){
            del_mods(MOD_MASK_SHIFT);
            register_code(KC_DEL);
            delkey_registered = true;
            set_mods(mod_state);
            return false;
          }
        } else {
          if(delkey_registered){
            unregister_code(KC_DEL);
            delkey_registered = false;
            return false;
          }
        }
        return true;
     }
  }
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record){
  switch(keycode){
    case (LSHIFT):
      return 50;
    case (OBJ_ARROW):
      return 50;
    default:
      return TAPPING_TERM;
  }
}

td_state_t cur_dance(qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)
        return TD_SINGLE_TAP;
    else
        return TD_SINGLE_HOLD;
  }
  else if (state->count == 2) {
      return TD_DOUBLE_TAP;
	}
	else if (state->count == 3) {
        return TD_TRIPLE_TAP;
	}
  else
      return TD_UNKNOWN;
}

static td_tap_t shifttap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void s_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           register_code(KC_S);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           register_code(KC_S);
           break;
      case TD_DOUBLE_TAP: break;
      case TD_TRIPLE_TAP: break;
      case TD_UNKNOWN: break;
      case TD_NONE: break;
  }
}

void s_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: unregister_code(KC_S); break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        unregister_code(KC_S);
        break;
    case TD_DOUBLE_TAP: break;
    case TD_TRIPLE_TAP: break;
    case TD_UNKNOWN: break;
    case TD_NONE: break;
  }
  shifttap_state.state = TD_NONE;
}

void copy_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           register_code(KC_C);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           register_code(KC_C);
           break;
      case TD_DOUBLE_TAP: break;
      case TD_TRIPLE_TAP: break;
      case TD_UNKNOWN: break;
      case TD_NONE: break;
  }
}

void copy_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: unregister_code(KC_C); break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        unregister_code(KC_C);
        break;
    case TD_DOUBLE_TAP: break;
    case TD_TRIPLE_TAP: break;
    case TD_UNKNOWN: break;
    case TD_NONE: break;
  }
  shifttap_state.state = TD_NONE;
}

void new_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           register_code(KC_N);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           register_code(KC_N);
           break;
      case TD_DOUBLE_TAP: break;
      case TD_TRIPLE_TAP: break;
      case TD_UNKNOWN: break;
      case TD_NONE: break;
  }
}

void new_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: unregister_code(KC_N); break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        unregister_code(KC_N);
        break;
    case TD_DOUBLE_TAP: break;
    case TD_TRIPLE_TAP: break;
    case TD_UNKNOWN: break;
    case TD_NONE: break;
  }
  shifttap_state.state = TD_NONE;
}

void find_page_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_P);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_P);
           break;
      case TD_DOUBLE_TAP: break;
      case TD_TRIPLE_TAP: break;
      case TD_UNKNOWN: break;
      case TD_NONE: break;
  }
}

void find_page_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        break;
    case TD_DOUBLE_TAP: break;
    case TD_TRIPLE_TAP: break;
    case TD_UNKNOWN: break;
    case TD_NONE: break;
  }
  shifttap_state.state = TD_NONE;
}

void obj_arrow_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           register_code16(KC_GT);
           break;
      case TD_SINGLE_HOLD:
           SEND_STRING("->");
           break;
      case TD_DOUBLE_TAP: break;
      case TD_TRIPLE_TAP: break;
      case TD_UNKNOWN: break;
      case TD_NONE: break;
  }
}

void obj_arrow_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: 
      unregister_code16(KC_GT);
      break;
    case TD_SINGLE_HOLD:
        break;
    case TD_DOUBLE_TAP: break;
    case TD_TRIPLE_TAP: break;
    case TD_UNKNOWN: break;
    case TD_NONE: break;
  }
  shifttap_state.state = TD_NONE;
}

void shift_brace_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
          register_code16(KC_LPRN);
          break;
      case TD_SINGLE_HOLD:
          register_code(KC_LSFT);
          break;
      case TD_DOUBLE_TAP:
          register_code16(KC_LCBR);
          break;
      case TD_TRIPLE_TAP:
          register_code16(KC_LBRC);
          break;
      case TD_UNKNOWN: break;
      case TD_NONE: break;
  }
}

void shift_brace_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code16(KC_LPRN);
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_LSFT);
      break;
    case TD_DOUBLE_TAP:
      unregister_code16(KC_LCBR);
      break;
    case TD_TRIPLE_TAP:
      unregister_code16(KC_LBRC);
      break;
    case TD_UNKNOWN: break;
    case TD_NONE: break;
  }
  shifttap_state.state = TD_NONE;
}

#ifdef TAP_DANCE_ENABLE
qk_tap_dance_action_t tap_dance_actions[] = {
  //[TD_LSHIFT_PAREN] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT_PAREN, KC_LSFT),
  [SAVE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, s_finished, s_reset),
  [COPY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, copy_finished, copy_reset),
  [NEW] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, new_finished, new_reset),
  [LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, shift_brace_finished, shift_brace_reset),
  [FIND_PAGE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, find_page_finished, find_page_reset),
  [OBJ_ARROW] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, obj_arrow_finished, obj_arrow_reset)

//   [] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,,),
};
#endif
