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

#include "encoder.h"

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
