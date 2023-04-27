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
    _MOUSE,
};

enum{
    SAVE,
    COPY,
    PASTE,
    CUT,
    NEW,
    NEW_TAB,
    LSHIFT,
    FIND_PAGE,
    OBJ_ARROW,
    DLR_THIS,
    TERMINAL,
    CLOSE_EXPLORER,
    FIND,
    HOME_SHIFT_END,
    UNDO,
    REDO,
    ALL,
    ALT_DOWN,
    ALT_UP,
	  COMMENT_LINE,
    JIRA,
    MR,
};

typedef enum {
	TD_NONE,
	TD_SINGLE_TAP,
	TD_SINGLE_HOLD,
	TD_DOUBLE_TAP,
  TD_DOUBLE_HOLD,
	TD_TRIPLE_TAP,
  TD_UNKNOWN,
} td_state_t;

typedef struct {
	bool is_press_action;
	td_state_t state;
} td_tap_t;

uint16_t key_timer;

td_state_t cur_dance(qk_tap_dance_state_t *state);

void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);

bool is_alt_tab_active = false; // ADD this near the begining of keymap.c
bool is_ctrl_tab_active = false; // ADD this near the begining of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.
uint16_t ctrl_tab_timer = 0;     // we will be using them soon.
enum custom_keycodes {          // Make sure have the awesome keycode ready
    ALT_TAB = SAFE_RANGE,
    SAVE_TAB,
    CTRL_PGUP,
    CTRL_PGDN,
    CTRL_BACK,
    CTRL_DELETE,
    CTRL_TAB,
    REFRESH,
    REFRESH_FRAME,
    CTRL_F2,
    COLLAPSE,
    UNCOLLAPSE,
};

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
 * |  Tab   | ' "  | , <  | . >  |   P  |   Y  |                              |   F  |   G  |   C  |   R  |   L  | Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   O  |   E  |   U  |   I  |                              |   D  |   H  |   T  |   N  |   S  |Enter   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift | ; :  |   Q  |   J  |   K  |   X  | [ {  |alttab|  |F-keys|  ] } |   B  |   M  |   W  |   V  |   Z  | ?      |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | SAVE |   ←  |  →   | Space| Sym  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *            ueu            `----------------------------------'  `----------------------------------'
 */
    [_DVORAK] = LAYOUT(
     TD(TERMINAL),KC_QUOTE,KC_COMM,  KC_DOT,   TD(FIND_PAGE) ,   TD(REDO) ,                                        TD(FIND),   KC_G ,  TD(COPY) ,  KC_R ,  KC_L, KC_DEL,
     CTL_ESC, TD(ALL),  KC_O,  KC_E,   KC_U ,   KC_I,                                       KC_D,   KC_H , KC_T,  KC_N,  KC_S , KC_ENT,
     KC_LSFT, KC_SCLN, TD(MR),  TD(JIRA)  ,   KC_K ,   TD(CUT) , KC_LBRC, ALT_TAB,     FKEYS  , KC_RBRC, TD(CLOSE_EXPLORER),   KC_M ,  KC_W,   TD(PASTE) ,  TD(UNDO), KC_QUES,
                                 SAVE, KC_LEFT, KC_RGHT, KC_SPC , MO(_SYM),     MO(_NAV), KC_BSPC ,KC_UP, KC_DOWN, ALT_TAB
    ),
/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    `   |  *   |  +   |  <   |  >   |  #  |                               |   [  |  7   |  8   |  9   |  ]   |   &    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |RefFrame|  /   |  =   |  !   |  $   |  _   |                              |   (  |  4   |  5   |  6   |  )   |   |    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |SAVE_TAB|   \  |  `   |  %   |  -   |  @   |  {   |      |  |      |   .  |   {  |  1   |  2   |  3   |  }   |   ?    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |  Nav |      |  0   |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
      KC_GRV ,   KC_ASTR ,   KC_PLUS ,   KC_LT ,   TD(OBJ_ARROW),   KC_HASH,                                        KC_LBRC,   KC_7 ,   KC_8 ,   KC_9 ,   KC_RBRC , KC_AMPR ,
     REFRESH_FRAME, TD(COMMENT_LINE),  KC_EQL, KC_EXLM,  TD(DLR_THIS), KC_UNDS,                                     KC_LPRN, KC_4, KC_5, KC_6, KC_RPRN, KC_PIPE,
     SAVE_TAB , KC_BSLS, KC_GRV, KC_PERC, KC_MINS, KC_AT, KC_LBRC, _______, _______, KC_DOT, KC_LCBR, KC_1, KC_2,  KC_3, KC_RCBR, KC_QUES,
                                  _______, _______, _______, _______, TO(_NAV), _______, _______, KC_0 , KC_DOT, _______
    ),
/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |uncollapse|collapse|                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      | sc+<-| sc+->|ALT_UP|                              | PgDn |ctrl+←|   ↓  |ctrl+→| VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |ctrlf2|LTDOWN|      |      |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      | CTRL | ctrl |      |      |  |      |      |      |      |      |
 *                        |      | Back |delete|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, UNCOLLAPSE, COLLAPSE,                                     KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_VOLU, KC_DEL,
      _______, _______, _______, RCS(KC_LEFT), RCS(KC_RIGHT), TD(ALT_UP),                                     KC_PGDN, RCTL(KC_LEFT), KC_DOWN, RCTL(KC_RGHT), KC_VOLD, KC_INS,
      _______, _______, _______, _______, CTRL_F2, TD(ALT_DOWN), _______, _______, _______, _______,KC_PAUSE, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_PSCR,
                                 _______, CTRL_BACK, CTRL_DELETE, _______, TD(HOME_SHIFT_END), TO(_DVORAK), _______, _______, _______, _______
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
bool oled_task_user(void) {
    // Host Keyboard Layer Status
    // oled_write_P(PSTR("Layer: "), false);
  if(is_keyboard_master()){
    switch (get_highest_layer(layer_state)) {
        case _DVORAK:
            oled_write_P(PSTR("Dvorak\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("Sym\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("Nav\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }
  }
  if(is_keyboard_left()){
       switch (get_highest_layer(layer_state)) {
        case _DVORAK:
            oled_write_P(PSTR("Dvorak\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("Sym\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("Nav\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
       }
  }

    // Host Keyboard LED Status
    // led_t led_state = host_keyboard_led_state();
    // oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    // oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    // oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    return false;
}
#endif
// #ifdef OLED_ENABLE
// bool oled_task_user(void) {
//     // Host Keyboard Layer Status
//     oled_write_P(PSTR("Layer: "), false);

//     switch (get_highest_layer(layer_state)) {
//         case _DVORAK:
//             oled_write_P(PSTR("Default\n"), false);
//             break;
//         // case _FN:
//         //     oled_write_P(PSTR("FN\n"), false);
//         //     break;
//         // case _ADJ:
//         //     oled_write_P(PSTR("ADJ\n"), false);
//         //     break;
//         default:
//             // Or use the write_ln shortcut over adding '\n' to the end of your string
//             oled_write_ln_P(PSTR("Undefined"), false);
//     }

//     // Host Keyboard LED Status
//     led_t led_state = host_keyboard_led_state();
//     oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
//     oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
//     oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

//     return false;
// }
// oled_rotation_t oled_init_user(oled_rotation_t rotation) {
//   if(!is_keyboard_master()){
//      return OLED_ROTATION_180;
//   }
//   return rotation;
// }

// bool oled_task_user(void){

    // oled_write_P(PSTR("Layer:"), false);

    // switch (get_highest_layer(layer_state)){
    //     case _DVORAK:
            // oled_write_P(PSTR("Default\n"), false);
    //         break;
    //     case _SYM:
    //         oled_write_P(PSTR("SYM\n"), false);
    //         break;
    //     default:
    //         oled_write_ln_P(PSTR("Undefined"), false);
    // }
    //     led_t led_state = host_keyboard_led_state();
    //     oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    //     oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    //     oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
// }
// #endif

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
        if(IS_LAYER_ON(_DVORAK)){
          if(clockwise){
            //select rightwards
            register_code(KC_RCTL);
            tap_code(KC_PGUP);
          }else{
            register_code(KC_RCTL);
            tap_code(KC_PGDN);
            //select leftwards
          }
        unregister_code(KC_RCTL);
          //on click select word
      }

    }
    //RIGHT ENCODER
    else if (index == 1) {
      if(IS_LAYER_ON(_DVORAK)){
        if (clockwise) {
          if (is_alt_tab_active) {
            tap_code(KC_LEFT);
            alt_tab_timer = timer_read();
          } else {
            //DEFAULT ENCODER
            tap_code(KC_PGDN);
          }
        } else {
          if (is_alt_tab_active) {
            alt_tab_timer = timer_read();
            tap_code(KC_RIGHT);
          } else {
            //DEFAULT ENCODER
            tap_code(KC_PGUP);
          }
        }
      }else{
        // DEFAULT Page up/Page down
        if (clockwise) {
            register_code(KC_RCTL);
            tap_code(KC_Y);
            unregister_code(KC_RCTL);
        } else {
            register_code(KC_RCTL);
            tap_code(KC_Z);
            unregister_code(KC_RCTL);
        }
      }
    }
    return false;
}
#endif

uint8_t mod_state;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  mod_state = get_mods();
  switch (keycode) { // This will do most of the grunt work with the keycodes.
    // case KC_UP:
    // case KC_DOWN:
    //   if (is_ctrl_tab_active) {
    //     ctrl_tab_timer = timer_read();
    //   }else{
    //     tap_code(keycode);
    //   }
    //   break;
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
    case REFRESH:
      if(record->event.pressed){
        register_code(KC_RCTL);
        tap_code(KC_R);
      }
      unregister_code(KC_RCTL);
      break;
    case REFRESH_FRAME:
      if(record->event.pressed){
        register_code(KC_RSFT);
        tap_code(KC_F10);
        unregister_code(KC_RSFT);
        tap_code(KC_H);
        tap_code(KC_R);
      }
      break;
    case SAVE:
        register_code(KC_RCTL);
        tap_code(KC_S);
        unregister_code(KC_RCTL);
      break;
    case CTRL_BACK:
        if (record->event.pressed) {
          register_code(KC_RCTL);
          tap_code(KC_BSPC);
        }
        unregister_code(KC_RCTL);
      break;
    case CTRL_DELETE:
      if (record->event.pressed) {
        register_code(KC_RCTL);
        tap_code(KC_DEL);
      }
      unregister_code(KC_RCTL);
      break;
    case CTRL_F2:
      if (record->event.pressed) {
        register_code(KC_RCTL);
        tap_code(KC_F2);
      }
      unregister_code(KC_RCTL);
      break;
    case COLLAPSE:
      if (record->event.pressed) {
        register_code(KC_RCTL);
        tap_code(KC_K);
        unregister_code(KC_RCTL);
        register_code(KC_RCTL);
        tap_code(KC_0);
      }
        unregister_code(KC_RCTL);
      break;
    case UNCOLLAPSE:
      if (record->event.pressed) {
        register_code(KC_RCTL);
        tap_code(KC_K);
        unregister_code(KC_RCTL);
        register_code(KC_RCTL);
        tap_code(KC_J);
      }
        unregister_code(KC_RCTL);
      break;
    // case CTRL_TAB:
    //   if (record->event.pressed) {
    //     if (!is_ctrl_tab_active) {
    //       is_ctrl_tab_active = true;
    //       register_code(KC_RCTL);
    //       ctrl_tab_timer = timer_read();
    //       register_code(KC_TAB);
    //     }
    //   }else {
    //     unregister_code(KC_RCTL);
    //   }
    //   break;
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
            register_code(KC_RSFT);
            tap_code(KC_F10);
            unregister_code(KC_RSFT);
            tap_code(KC_H);
            tap_code(KC_R);
        }
        break;
    case KC_BSPC:{
        //https://beta.docs.qmk.fm/using-qmk/simple-keycodes/feature_advanced_keycode
        bool delkey_registered = false;
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
  // if (is_ctrl_tab_active) {
  //   if (timer_elapsed(ctrl_tab_timer) > 1000) {
  //     unregister_code(KC_TAB);
  //     is_ctrl_tab_active = false;
  //   }
  // }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record){
  switch(keycode){
    case (LSHIFT):
      return 50;
    case (OBJ_ARROW):
      return 100;
    case (DLR_THIS):
    case (NEW):
      return 500;
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
    if (state->interrupted || !state->pressed)
      return TD_DOUBLE_TAP;
    else
      return TD_DOUBLE_HOLD;
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
           tap_code(KC_S);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_S);
           break;
      default:
        break;
  }
}

void s_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        break;
    default:
      break;
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
    default:
      break;
  }
}

void copy_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: unregister_code(KC_C); break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        unregister_code(KC_C);
        break;
    default:
      break;
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
      default:
        break;
  }
}

void new_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: unregister_code(KC_N); break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        unregister_code(KC_N);
        break;
    default:
      break;
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
      default:
        break;
  }
}

void find_page_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP: break;
    case TD_SINGLE_HOLD:
        unregister_code(KC_RCTL);
        break;
    default:
      break;
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
      case TD_DOUBLE_TAP:
          SEND_STRING(" => ");
           break;
      default:
        break;
  }
}

void obj_arrow_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code16(KC_GT);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void jira_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_J);
           break;
      case TD_SINGLE_HOLD:
           SEND_STRING("https://jira.g2planet.com/browse/EC-");
           break;
      case TD_DOUBLE_TAP:
           break;
      default:
        break;
  }
}

void jira_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void mr_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_Q);
           break;
      case TD_SINGLE_HOLD:
           SEND_STRING("https://git.g2planet.com/g2planet/eclib/-/merge_requests/");
           break;
      case TD_DOUBLE_TAP:
           break;
      default:
        break;
  }
}

void mr_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void dlr_this_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           register_code16(KC_DLR);
           break;
      case TD_SINGLE_HOLD:
           SEND_STRING("$this->");
           break;
      case TD_DOUBLE_TAP:
          SEND_STRING("$this->request");
           break;
      case TD_DOUBLE_HOLD:
          SEND_STRING("$this->request['']");
          break;
      default:
        break;
  }
}

void dlr_this_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code16(KC_DLR);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void terminal_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_TAB);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_GRV);
           break;
      default:
        break;
  }
}

void terminal_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code16(KC_RCTL);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void close_explorer_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_B);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_B);
           break;
      default:
        break;
  }
}

void close_explorer_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code16(KC_RCTL);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void paste_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_V);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_V);
           break;
      case TD_DOUBLE_TAP:
           register_code(KC_LWIN);
           tap_code(KC_V);
           break;
      default:
        break;
  }
}

void paste_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_RCTL);
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_LWIN);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void cut_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_X);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_X);
           break;
      default:
        break;
  }
}

void cut_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_RCTL);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void redo_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_Y);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_Y);
           break;
      default:
        break;
  }
}

void redo_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_RCTL);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void alt_tab_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           register_code(KC_LALT);
           tap_code(KC_TAB);
           break;
      default:
        break;
  }
}

void alt_tab_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code(KC_LALT);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void undo_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_Z);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           tap_code(KC_Z);
           break;
      default:
        break;
  }
}

void undo_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_RCTL);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void all_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_A);
           break;
      case TD_DOUBLE_TAP:
           register_code(KC_RCTL);
           tap_code(KC_A);
           break;
      default:
        break;
  }
}

void all_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_RCTL);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void find_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_F);
           break;
      case TD_SINGLE_HOLD:
           register_code(KC_RCTL);
           register_code(KC_RSFT);
           tap_code(KC_F);
           break;

      default:
        break;
  }
}

void find_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_RCTL);
      unregister_code(KC_RSFT);
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void home_shift_end_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
           tap_code(KC_HOME);
           register_code(KC_RSFT);
           tap_code(KC_END);
           break;
      case TD_SINGLE_HOLD:
           tap_code(KC_END);
           tap_code(KC_SCLN);
           break;
      default:
        break;
  }
}

void home_shift_end_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code(KC_RSFT);
      break;
    case TD_SINGLE_HOLD:
      break;
    case TD_DOUBLE_TAP:
    default:
      break;
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
      case TD_DOUBLE_HOLD:
        break;
      case TD_TRIPLE_TAP:
          register_code16(KC_LBRC);
          break;
      default:
        break;
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
    case TD_DOUBLE_HOLD:
      break;
    case TD_TRIPLE_TAP:
      unregister_code16(KC_LBRC);
      break;
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void alt_down_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
          register_code(KC_LALT);
          tap_code(KC_DOWN);
          break;
      case TD_SINGLE_HOLD:
          register_code(KC_RSFT);
          register_code(KC_LALT);
          tap_code(KC_DOWN);
          break;
      default:
        break;
  }
}

void alt_down_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code(KC_LALT);
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_LALT);
      unregister_code(KC_RSFT);
      break;
    case TD_DOUBLE_TAP:
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void alt_up_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
          register_code(KC_LALT);
          tap_code(KC_UP);
          break;
      case TD_SINGLE_HOLD:
          register_code(KC_RSFT);
          register_code(KC_LALT);
          tap_code(KC_UP);
          break;
      default:
        break;
  }
}

void alt_up_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_TAP:
      unregister_code(KC_LALT);
      break;
    case TD_SINGLE_HOLD:
      unregister_code(KC_LALT);
      unregister_code(KC_RSFT);
      break;
    case TD_DOUBLE_TAP:
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}

void comment_line_finished(qk_tap_dance_state_t *state, void *user_data) {
  shifttap_state.state = cur_dance(state);
    switch (shifttap_state.state) {
      case TD_SINGLE_TAP:
          tap_code(KC_SLSH);
          break;
      case TD_SINGLE_HOLD:
          register_code(KC_RCTL);
          tap_code(KC_SLSH);
          break;
      default:
        break;
  }
}

void comment_line_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (shifttap_state.state) {
    case TD_SINGLE_HOLD:
      unregister_code(KC_RCTL);
      break;
    case TD_DOUBLE_TAP:
	case TD_SINGLE_TAP:
    default:
      break;
  }
  shifttap_state.state = TD_NONE;
}


#ifdef TAP_DANCE_ENABLE
qk_tap_dance_action_t tap_dance_actions[] = {
  [SAVE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, s_finished, s_reset),
  [COPY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, copy_finished, copy_reset),
  [PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, paste_finished, paste_reset),
  [CUT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cut_finished, cut_reset),
  [NEW] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, new_finished, new_reset),
  [UNDO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, undo_finished, undo_reset),
  [REDO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, redo_finished, redo_reset),
  [ALL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, all_finished, all_reset),
  [JIRA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, jira_finished, jira_reset),
  [MR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mr_finished, mr_reset),
  [LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, shift_brace_finished, shift_brace_reset),
  [FIND_PAGE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, find_page_finished, find_page_reset),
  [OBJ_ARROW] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, obj_arrow_finished, obj_arrow_reset),
  [DLR_THIS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dlr_this_finished, dlr_this_reset),
  [TERMINAL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, terminal_finished, terminal_reset),
  [CLOSE_EXPLORER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, close_explorer_finished, close_explorer_reset),
  [FIND] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, find_finished, find_reset),
  [HOME_SHIFT_END] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, home_shift_end_finished, home_shift_end_reset),
  [ALT_DOWN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_down_finished, alt_down_reset),
  [ALT_UP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_up_finished, alt_up_reset),
  [COMMENT_LINE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, comment_line_finished, comment_line_reset),

//   [] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,,),
};
#endif
