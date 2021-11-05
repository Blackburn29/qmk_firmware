/* Copyright 2021 Blackburn29
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
#include "rgb_matrix_map.h"

uint8_t last_rgb_mode;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    //      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Prt           Rotary(Mute)
    //      ~        1        2        3        4        5        6        7        8        9        0         -       (=)      BackSpc           Del
    //      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
    //      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
    //      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
    //      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(
            KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,          KC_MUTE,
            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
            KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
            KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
            ),

    [1] = LAYOUT(
            _______, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, KC_PSCR,          _______,
            _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, RGB_SPD, RGB_VAI, RGB_SPI,  _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET,            _______,
            _______, KC_HOME, RGB_VAD, KC_END, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
            _______,          _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, KC_END,
            _______, _______, _______,                            _______,                            _______, _______, _______, KC_HOME, RGB_RMOD, KC_END
            ),


};
// clang-format on

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    // https://beta.docs.qmk.fm/using-qmk/simple-keycodes/feature_advanced_keycodes#alt-escape-for-alt-tab-id-alt-escape-for-alt-tab
    if (get_mods() & MOD_MASK_CTRL) { // If CTRL is held
        uint8_t mod_state = get_mods(); // Store all  modifiers that are held
        unregister_mods(MOD_MASK_CTRL); // Immediately unregister the CRTL key (don't send CTRL-PgDn) - del_mods doesn't work here (not immediate)
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
        set_mods(mod_state); // Add back in the CTRL key - so ctrl-key will work if ctrl was never released after paging.
    } else if (get_mods() & MOD_MASK_SHIFT) {
        uint8_t mod_state = get_mods();
        unregister_mods(MOD_MASK_SHIFT);  
        if (clockwise) {
            tap_code(KC_MS_WH_DOWN);
        } else {
            tap_code(KC_MS_WH_UP);
        }
        set_mods(mod_state);
    } else if (clockwise) { // All else volume.
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return true;
}
#endif //ENCODER_ENABLE

#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
#ifdef CONSOLE_ENABLE
    uprintf("Layer State: %d", layer_state);
#endif
    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        rgb_matrix_set_color(LED_CAPS, RGB_PURPLE);
    }
    if (keymap_config.no_gui) {
        rgb_matrix_set_color(LED_LWIN, RGB_PURPLE);  //light up Win key when disabled
    }

    for (int i = 0; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_WHITE);
        rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_WHITE);
    }

    switch(get_highest_layer(layer_state)){  
        case 1:  // on Fn pressed
            for (int i = 0; i < 8; i++) {
                rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_PURPLE);
                rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_PURPLE);
            }
            rgb_matrix_set_color(LED_UNDER_ENCODER[0], RGB_PURPLE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[1], RGB_PURPLE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[2], RGB_PURPLE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[3], RGB_PURPLE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[3], RGB_PURPLE); 
            rgb_matrix_set_color(LED_INS, RGB_PURPLE); 
            rgb_matrix_set_color(LED_A, RGB_PURPLE); 
            rgb_matrix_set_color(LED_D, RGB_PURPLE); 
            rgb_matrix_set_color(LED_LEFT, RGB_PURPLE); 
            rgb_matrix_set_color(LED_RIGHT, RGB_PURPLE); 
    }
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_MOD:
        case RGB_RMOD:
            last_rgb_mode = rgb_matrix_get_mode();
        break;

    }
    return true;
}

uint32_t layer_state_set_user(uint32_t state) {
 switch (biton32(state)) {
 case 1:
     rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
     break;
 default: //  for any other layers, or the default layer
     rgb_matrix_mode_noeeprom(last_rgb_mode);
     break;
    }
 return state;
}


void keyboard_post_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_flags(LED_FLAG_ALL);
    last_rgb_mode = rgb_matrix_get_mode();
#endif
}
