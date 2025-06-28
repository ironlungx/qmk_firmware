/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// #define USE_MATRIX_I2C
#define OLED_DRIVER

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// #define QUICK_TAP_TERM 20
// #define TAPPING_TERM 100

// Tapping term: The max time (in ms) to register a key as a tap
#define TAPPING_TERM 185

// #define MK_KINETIC_SPEED

// #define MOUSEKEY_DELAY          0
// #define MOUSEKEY_INTERVAL       7 // from 4 to 7
// #define MOUSEKEY_MAX_SPEED      2 // from 1 to 3
// #define MOUSEKEY_TIME_TO_MAX    32

#undef MOUSEKEY_DELAY
#undef MOUSEKEY_INTERVAL
#undef MOUSEKEY_WHEEL_DELAY
#undef MOUSEKEY_MAX_SPEED
#undef MOUSEKEY_TIME_TO_MAX

#define MOUSEKEY_DELAY          0
#define MOUSEKEY_INTERVAL       16
#define MOUSEKEY_WHEEL_DELAY    0
#define MOUSEKEY_MAX_SPEED      6
#define MOUSEKEY_TIME_TO_MAX    64

// Enables prioritizing hold if another key is pressed within the tapping term
#define PERMISSIVE_HOLD


#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX
