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

#include "rgb_matrix_effects.h"
#include "rgb_matrix_map.h"
#include "effects/fade.h"
#include "effects/blink.h"


void rgb_matrix_set_sidebar_color(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], red, green, blue);
        rgb_matrix_set_color(LED_SIDE_LEFT[i], red, green, blue);
    }
}

void rgb_matrix_handle_effects() {
    RGB rgbVal = get_fade_rgb_value_for_current_tick();


    for (int i = 0; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], rgbVal.r, rgbVal.g, rgbVal.b);
        rgb_matrix_set_color(LED_SIDE_LEFT[i], rgbVal.r, rgbVal.g, rgbVal.b);
    }

    // Mimic stock functionality of blinking sidebars on caps lock
    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        if (is_blink_active(200)) {
            rgb_matrix_set_color(LED_CAPS, RGB_RED);
            rgb_matrix_set_sidebar_color(RGB_RED);
        } else {
            rgb_matrix_set_color(LED_CAPS, RGB_BLACK);
            rgb_matrix_set_sidebar_color(RGB_BLACK);
        }
    } 

    // Highlight layer "helper" keys
    switch(get_highest_layer(layer_state)){  
        case 1:  // on Fn pressed
            for (int i = 0; i < 8; i++) {
                rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_AZURE);
                rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_AZURE);
            }
            rgb_matrix_set_color(LED_UNDER_ENCODER[0], RGB_AZURE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[1], RGB_AZURE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[2], RGB_AZURE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[3], RGB_AZURE); 
            rgb_matrix_set_color(LED_UNDER_ENCODER[3], RGB_AZURE); 
            rgb_matrix_set_color(LED_INS, RGB_AZURE); 
            rgb_matrix_set_color(LED_A, RGB_AZURE); 
            rgb_matrix_set_color(LED_D, RGB_AZURE); 
            rgb_matrix_set_color(LED_LEFT, RGB_AZURE); 
            rgb_matrix_set_color(LED_RIGHT, RGB_AZURE); 
    }
}
