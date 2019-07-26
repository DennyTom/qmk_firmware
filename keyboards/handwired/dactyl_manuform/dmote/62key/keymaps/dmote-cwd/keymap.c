#include "62key.h"
#include "rgblight.h"
#include <sendstring_colemak.h>

extern keymap_config_t keymap_config;

// Automatic Layer ID:
enum layer_names {
    _COLEMAK,   // Hardware Colemak, default.
    _QWERTY,  // Hardware QWERTY, for demoing to others.
    _GAME, //Gaming layer, for disabling game breaking mods.
    _NUMERIC, // For entering numbers.
    _MOVE // For movement.
};

// Shorthand:
#define LAYER_N TT(_NUMERIC)
#define LAYER_Q TG(_QWERTY)
#define LAYER_G TG(_GAME)
#define LAYER_M MO(_MOVE)
#define PASTE LSFT(KC_INS)  // Terminal-compatible paste.
#define BK_LCTL CTL_T(KC_LBRACKET)
#define BK_RCTL RCTL_T(KC_RBRACKET)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_COLEMAK] = LAYOUT_62key(
    KC_GRV,  KC_MINS, KC_W,    KC_F,    KC_P,    KC_G,
    KC_TAB,  KC_Q,    KC_R,    KC_S,    KC_T,    KC_D,
    KC_ESC,  KC_A,    KC_X,    KC_C,    KC_V,    KC_B,
    KC_LSPO, KC_Z,    KC_HOME, KC_PGUP, KC_END,
                               KC_PGDN,          KC_ENT,  KC_SPC,
                                             KC_LSPO, KC_LGUI, KC_MINS,
                                                 BK_LCTL, KC_LALT,

                 KC_J,    KC_L,    KC_U,    KC_Y,    KC_EQL,  KC_LEAD,
                 KC_H,    KC_N,    KC_E,    KC_I,    KC_SCLN, KC_BSLS,
                 KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_O,    KC_QUOT,
                          KC_LEFT, KC_UP,   KC_RGHT, KC_SLSH, KC_RSPC,
        KC_DEL,  KC_BSPC,          KC_DOWN,
    KC_EQL,  LAYER_N, KC_RSPC,
        KC_RALT, BK_RCTL
),

[_QWERTY] = LAYOUT_62key(
    _______, _______, KC_W,    KC_E,    KC_R,    KC_T,
    _______, KC_Q,    KC_S,    KC_D,    KC_F,    KC_G,
    _______, KC_A,    KC_X,    KC_C,    KC_V,    KC_B,
    _______, KC_Z,    _______, _______, _______,
                               _______,          _______, _______,
                                             _______, _______, _______,
                                                 _______, _______,

                 KC_Y,    KC_U,    KC_I,    KC_O,    _______, _______,
                 KC_H,    KC_J,    KC_K,    KC_L,    KC_P,    _______,
                 KC_N,    KC_M,    _______, _______, KC_SCLN, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,          _______,
    _______, _______, _______,
        _______, _______
),
    
[_GAME] = LAYOUT_62key(
    _______, _______, ______, ______,  ______, ____,
    _______, _______, ______, ______,  ______, ____,
    _______, _______, ______, ______,  ______, ____,
    KC_LSFT, _______, ______, ______,  ______,
                              ______,           KC_ENT,  KC_SPC,
                                             _______, _______, ______,
                                                 _______, _______,

                 ____,  _______, _______, _______, _______, _______,
                 ____,  _______, _______, _______, _______, _______,
                 ____,  _______, _______, _______, _______, _______,
                        _______, _______, _______, _______, KC_RSFT,
        ______,  ______,         _______,
    ______,  _______, ______,
        _______, ______
),

[_NUMERIC] = LAYOUT_62key(
    LAYER_Q, KC_INS,  KC_F2,   KC_F3,   KC_F4,   KC_F5,
    KC_F12,  KC_F1,   KC_2,    KC_3,    KC_4,    KC_5,
    _______, KC_1,    KC_AT,   KC_HASH, KC_DLR,  KC_PERC,
    KC_GRV,  KC_EXLM, _______, _______, _______,
                               _______,          RGB_MOD, _______,
                                             _______, _______, ______,
                                                 _______, _______,

                 KC_F6,   KC_F7,   KC_F8,   KC_F9,   RESET,   LAYER_G,
                 KC_6,    KC_7,    KC_8,    KC_9,    KC_F10,  KC_F11,
                 KC_CIRC, KC_AMPR, KC_ASTR, KC_APP,  KC_0,    PASTE,
                          _______, _______, _______, KC_PSCR, RGB_TOG,
        _______, _______,          _______,
    _______, _______, _______,
        _______, _______
),

[_MOVE] = LAYOUT_62key(
    KC_VOLD, KC_VOLU,  KC_HOME, KC_UP,    KC_PGUP,  ____,
    _______, _______,  KC_LEFT, KC_DOWN,  KC_RIGHT, ____,
    KC_INS,  _______,  KC_END,  _______,  KC_PGDN,  ____,
    _______, _______,  ______,  _______,  _______,
                               _______,          KC_DEL,  KC_BSPC,
                                             _______, _______, _______,
                                                 _______, _______,

                 ____,  _______, KC_UP,   _______, KC_MPLY, KC_MUTE,
                 ____,  KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
                 ____,  _______, _______, _______, _______, _______,
                        _______, _______, _______, _______, _______,
        KC_ENT,  KC_SPC,         _______,
    ______,  _______, ______,
        _______, ______
)

};

/*
The rest is all about lighting control.
The logic here represents a pretty poor compromise solution between the
following concerns:

- Feedback on active modifiers.
- Flexibility: Both sides of the keyboard are interchangeable.
- Regular QMK RBG lighting modes. Specifically, Knight and Xmas.

Currently, the last item suffers, because the first two seem to require
calling a function that implements the RGBLIGHT_SPLIT_SET_CHANGE_HSVS macro,
which most of the rgblight.c functions do not. In particular, functions that
target an individual LED do not do so correctly across the wire, so instead
we let HSV vary without ever targeting LEDs.
*/

// How long to wait between animation steps for "Knight" animation:
const uint8_t RGBLED_KNIGHT_INTERVALS[] PROGMEM = {255, 200, 100};

bool _initialized = false;
bool _leds_dirty = false;

void modal_leds(void) {
    uint8_t mods = get_mods();
    uint16_t hue = 355;  // Rough match to printed case.
    uint8_t saturation = 255;
    uint8_t value = 0;
    if (layer_state_is(_COLEMAK)) { hue -= 50; saturation -= 20; value += 20; };
    if (layer_state_is(_NUMERIC)) { value += 30; };
    if (mods & MOD_MASK_SHIFT) { saturation -= 20; value += 30; };
    if (mods & MOD_MASK_ALT) { hue -= 100; saturation -= 20; value += 30; };
    if (mods & MOD_MASK_CTRL) { hue -= 200; saturation -= 20; value += 30; };
    // rgblight_sethsv_eeprom_helper is not a great API function but it does
    // affect both halves of a split keyboard.
    rgblight_sethsv_eeprom_helper(hue, saturation, value, false);
    _leds_dirty = false;
}

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
    if (_leds_dirty) { modal_leds(); };
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!_initialized) {
        // Static lighting is amenable to customization.
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        _initialized = true;
    };
    if (keycode == KC_WAKE) {
        // Turn the lights off before going to sleep.
        rgblight_sethsv_eeprom_helper(0, 0, 0, false);
    } else {
        _leds_dirty = true;
    };
    return true;
}
