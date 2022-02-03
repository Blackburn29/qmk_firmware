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

#include "fade.h"
#include "math.h"

uint16_t fade_timer = 0;
uint8_t sidebar_color_hue;
bool fade_direction = true; // false - decrement, true, - increment
uint16_t num_ticks = 20;


RGB get_fade_rgb_value_for_current_tick(void) {
    fade_timer++;

    if (fade_timer > num_ticks) {

        if (sidebar_color_hue + 5 >= 255) {
            fade_direction = false;
        } else  if (sidebar_color_hue <= 0) {
            fade_direction = true;
        }

        if (fade_direction == true) {
            sidebar_color_hue = sidebar_color_hue + 5;
        } else {
            sidebar_color_hue = sidebar_color_hue - 5;
        }

        //uprintf("sidebar_color_hue: %d -- fade_timer: %d\n", sidebar_color_hue, fade_timer);

        fade_timer = 0;
    }

    double sinCurve = sin((sidebar_color_hue/255.0) * (M_PI/2));
    num_ticks = 40 * sinCurve;

    HSV requestedColor = {rgb_matrix_get_hue(), sidebar_color_hue, 255 * sinCurve};

    return hsv_to_rgb(requestedColor);
}
