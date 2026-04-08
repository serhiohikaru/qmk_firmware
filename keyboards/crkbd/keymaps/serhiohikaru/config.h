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

// RGB Light Configuration
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_VAL 120

// how long before a key press becomes a hold
#define TAPPING_TERM 300

// makes tap and hold keys trigger the hold if another key is pressed before
// releasing, even if it hasn't hit the TAPPING_TERM
// #define PERMISSIVE_HOLD

// selects the hold action of a dual-role key as soon as the tap of the
// dual-role key is interrupted by the press of another key.
#define HOLD_ON_OTHER_KEY_PRESS

// activate Caps Word by simultaneously pressing both shift keys
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// Holding and releasing a dual-function key without pressing another key will
// result in nothing happening. With retro tapping enabled, releasing the key
// without pressing another will send the original keycode even if it is
// outside the tapping term.
// #define RETRO_TAPPING
