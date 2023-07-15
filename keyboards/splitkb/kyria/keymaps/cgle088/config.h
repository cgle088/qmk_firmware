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

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_ANIMATIONS
#    define RGBLIGHT_HUE_STEP  8
#    define RGBLIGHT_SAT_STEP  8
#    define RGBLIGHT_VAL_STEP  8
#    define RGBLIGHT_LIMIT_VAL 150
#    define RGBLIGHT_LAYERS
#endif
#define ENCODER_RESOLUTIONS {2}
#define ENCODER_RESOLUTIONS_RIGHT {2}
#define ENCODER_DEFAULT_POS 0x3
#define SPLIT_HAND_PIN_LOW_IS_LEFT
#define PERMISSIVE_HOLD

#define OLED_DISPLAY_128X64
#define OLED_FONT_WIDTH 6
#define OLED_FONT_HEIGHT 8
#define TAPPING_TERM 150

#define MASTER_RIGHT
// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
// #define SPLIT_USB_DETECT
// #define NO_USB_STARTUP_CHECK
