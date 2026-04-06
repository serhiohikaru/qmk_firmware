#include QMK_KEYBOARD_H
#include "eeprom.h"

#define HEATMAP_EEPROM_ADDR 32
#define SAVE_INTERVAL 15000

uint16_t heatmap[RGB_MATRIX_LED_COUNT];
uint32_t save_timer;

// ---------- EEPROM ----------
void heatmap_load(void) {
    eeprom_read_block((void*)heatmap,
                      (const void*)HEATMAP_EEPROM_ADDR,
                      sizeof(heatmap));
}

void heatmap_save(void) {
    eeprom_update_block((const void*)heatmap,
                        (void*)HEATMAP_EEPROM_ADDR,
                        sizeof(heatmap));
}

// ---------- Fixed ranges → solid colors ----------
rgb_t heat_to_rgb(uint16_t v) {
    // example ranges (edit freely)
    if (v < 1)   return (rgb_t){0, 0, 0};       // OFF
    if (v < 10)  return (rgb_t){0, 0, 255};     // BLUE
    if (v < 100)  return (rgb_t){0, 255, 0};     // GREEN
    if (v < 8000)  return (rgb_t){255, 255, 0};   // YELLOW
    if (v < 20000) return (rgb_t){255, 128, 0};   // ORANGE
    return (rgb_t){255, 0, 0};                    // RED
}

// ---------- Keypress ----------
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t row = record->event.key.row;
        uint8_t col = record->event.key.col;

        uint8_t index = g_led_config.matrix_co[row][col];
        if (index != NO_LED) {
            heatmap[index]++;
        }
    }
    return true;
}

// ---------- LED Rendering ----------
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_t c = heat_to_rgb(heatmap[i]);
        RGB_MATRIX_INDICATOR_SET_COLOR(i, c.r, c.g, c.b);
    }
    return false;
}

// ---------- Save ----------
void matrix_scan_user(void) {
    if (timer_elapsed32(save_timer) > SAVE_INTERVAL) {
        heatmap_save();
        save_timer = timer_read32();
    }
}

// ---------- Init ----------
void keyboard_post_init_user(void) {
    heatmap_load();
    save_timer = timer_read32();

    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
}
enum layers {
    _BASE,
    _NAV,
    _NUM,
    _SYM,
    _FUN
};


// Home Row Mods
#define HM_A LGUI_T(KC_A)
#define HM_S LSFT_T(KC_S)
#define HM_D LALT_T(KC_D)
#define HM_F LCTL_T(KC_F)

#define HM_J RCTL_T(KC_J)
#define HM_K RALT_T(KC_K)
#define HM_L RSFT_T(KC_L)
#define HM_OE RGUI_T(KC_SCLN)

const uint16_t PROGMEM combo_tab[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM combo_spc[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_esc[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM combo_ent[] = {KC_COMM, KC_DOT, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_tab, KC_TAB),
    COMBO(combo_spc, KC_SPC),
    COMBO(combo_esc, KC_ESC),
    COMBO(combo_ent, KC_ENT),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// BASE
[_BASE] = LAYOUT_split_3x6_3(
KC_NO, KC_Q, KC_W, KC_E, KC_R, KC_T,       KC_Z, KC_U, KC_I, KC_O, KC_P, KC_NO,
KC_NO, HM_A, HM_S, HM_D, HM_F, KC_G,       KC_H, HM_J, HM_K, HM_L, HM_OE, KC_NO,
KC_NO, KC_Y, KC_X, KC_C, KC_V, KC_B,       KC_N, KC_M, KC_COMM, KC_DOT, KC_MINS, KC_NO,

                LT(_NAV, KC_TAB), LT(_SYM, KC_SPC), LT(_NUM, KC_BSPC),
                LT(_FUN, KC_ESC), LT(_SYM, KC_ENT), KC_DEL
),

// NAV + Mouse (Vim Style)
[_NAV] = LAYOUT_split_3x6_3(
_______, _______, _______, MS_BTN4, KC_PGUP, _______,   MS_LEFT, MS_DOWN, MS_UP, MS_RGHT, _______, _______,
_______, _______, _______, KC_HOME,  KC_END, _______,   KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______, _______,
_______, _______, _______, MS_BTN5, KC_PGDN, _______,   MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, _______, _______,

                _______, _______, _______,
                MS_BTN1, MS_BTN2, MS_BTN3
),

// NUM
[_NUM] = LAYOUT_split_3x6_3(
_______, _______, _______, _______, _______, _______, _______,   KC_7,    KC_8,    KC_9, _______, _______,
_______, _______, _______, _______, _______, _______, _______,   KC_4,    KC_5,    KC_6, _______, _______,
_______, _______, _______, _______, _______, _______, _______,   KC_1,    KC_2,    KC_3, _______, _______,

                _______, _______, _______,
                KC_0, _______, _______
),

// SYMBOLS
[_SYM] = LAYOUT_split_3x6_3(
_______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
_______, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PIPE,   KC_PLUS, KC_MINS, KC_EQL,  KC_LT,   KC_GT,   _______,
_______, KC_GRV,  KC_TILD, KC_BSLS, KC_SLSH, KC_UNDS,   KC_COLN, KC_SCLN, KC_DQUO, KC_QUOT, KC_QUES, _______,

                _______, _______, _______,
                _______, _______, _______
),

// F KEYS
[_FUN] = LAYOUT_split_3x6_3(
_______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,   KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, _______,
_______, RM_TOGG, RM_NEXT, _______, DT_UP, DT_DOWN,   KC_F11, KC_F12, _______, _______, _______, _______,
_______, CM_TOGG, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,

                _______, _______, _______,
                _______, _______, _______
)

};
