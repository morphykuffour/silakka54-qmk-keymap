// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H


enum custom_keycodes {
    CKC_COPY = SAFE_RANGE,
    CKC_UPDIR,
    CKC_PASTE,
    CKC_SCOPE,
    CKC_CUT,
    JIGGLE,
    SELLINE,
    SELALL,
    DELWORD,
    SMTD_KEYCODES_BEGIN,
    CKC_R, // reads as C(ustom) + KC_A
    CKC_I,
    CKC_SPACE, // Custom space key for MEH+SPACE
    SMTD_KEYCODES_END,
};

#include "sm_td/sm_td.h"

// #define MEH_SPACE MT(MOD_MEH, KC_SPACE)
#define CKC_CTRL MT(MOD_LCTL,KC_ESC)
#define CKC_SLSH LSFT_T(KC_SLSH)
#define CUSTOM_TAP_DELAY 10

// Some helper C macros
    #define GENERAL_MODIFIER_KEY_DELAY_MS 20
    #define GENERAL_KEY_ACTION_DELAY_MS   50

    #define KEY_MODIFIER_ACTION(keycode, modifier) \
        SS_DOWN(modifier) \
        SS_DELAY(GENERAL_MODIFIER_KEY_DELAY_MS) \
        SS_TAP(keycode) \
        SS_DELAY(GENERAL_KEY_ACTION_DELAY_MS) \
        SS_UP(modifier) \
        SS_DELAY(GENERAL_MODIFIER_KEY_DELAY_MS)

    #define KEY_CTRL_ACTION(keycode) \
        KEY_MODIFIER_ACTION(keycode,X_LCTL)

    #define KEY_APPLE_KEY_ACTION(keycode) \
        KEY_MODIFIER_ACTION(keycode,X_LCMD)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                               KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, DELWORD,
        CKC_CTRL,KC_A,    CKC_R,    KC_S,    KC_T,    KC_G,                              KC_M,    KC_N,    KC_E,    CKC_I,   KC_O,    KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                               KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
                                            KC_LGUI, MO(1), KC_BSPC,              CKC_SPACE,  QK_REP,  KC_RALT
    ),
    [1] = LAYOUT(
        KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_PGUP, KC_PGDN, KC_HOME, KC_END,  KC_DEL,  KC_F12,
        KC_TRNS,  SELALL,  SELLINE, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_LBRC, KC_RBRC,
        KC_TRNS,  KC_TRNS, CKC_CUT, CKC_COPY,KC_TRNS, CKC_PASTE,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,  MO(3),  KC_TRNS
    ),
    [2] = LAYOUT(
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_GRV,  KC_LABK, KC_RABK, KC_QUOT, KC_DOT,                             KC_AMPR,  CKC_SCOPE, KC_LBRC, KC_RBRC, KC_PERC, KC_TRNS,
        KC_TRNS,  KC_EXLM, KC_MINS, KC_PLUS, KC_EQL,  KC_HASH,                            KC_PIPE,  KC_COLN, KC_LPRN, KC_RPRN, KC_QUES, KC_TRNS,
        KC_TRNS,  KC_CIRC, CKC_SLSH,KC_ASTR, KC_BSLS, CKC_UPDIR,                          KC_TILD,  KC_DLR,  KC_LCBR, KC_RCBR, KC_AT, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS
    ),
    [3] = LAYOUT(
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT,
                                            KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS
    )
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        static deferred_token token = INVALID_DEFERRED_TOKEN;
        static report_mouse_t report = {0};
        if (token) {
            // If jiggler is currently running, stop when any key is pressed.
            cancel_deferred_exec(token);
            token = INVALID_DEFERRED_TOKEN;
            report = (report_mouse_t){};  // Clear the mouse.
            host_mouse_send(&report);

        } else if (keycode == JIGGLE) {

            uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
                // Deltas to move in a circle of radius 20 pixels over 32 frames.
                static const int8_t deltas[32] = {
                    0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
                    0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0};
                static uint8_t phase = 0;
                // Get x delta from table and y delta by rotating a quarter cycle.
                report.x = deltas[phase];
                report.y = deltas[(phase + 8) & 31];
                phase = (phase + 1) & 31;
                host_mouse_send(&report);
                return 16;  // Call the callback every 16 ms.
            }
            token = defer_exec(1, jiggler_callback, NULL);  // Schedule callback.
        }
    }

    if (!process_smtd(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case CKC_COPY:
            if (record->event.pressed) {
                #if defined(OS_DETECTION_ENABLE)
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Mac: Cmd + C
                    SEND_STRING(KEY_APPLE_KEY_ACTION(X_C));
                }
                else {
                    // Linux, Windows, etc.: Ctrl + C
                    SEND_STRING(KEY_CTRL_ACTION(X_C));
                }
                #endif
            }
            break;

        case CKC_PASTE:
            if (record->event.pressed) {
                #if defined(OS_DETECTION_ENABLE)
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Mac: Cmd + V
                    SEND_STRING(KEY_APPLE_KEY_ACTION(X_V));
                }
                else {
                    // Linux, Windows, etc.: Ctrl + V
                    SEND_STRING(KEY_CTRL_ACTION(X_V));
                }
                #endif
            }
            break;

        case CKC_CUT:
            if (record->event.pressed) {
                #if defined(OS_DETECTION_ENABLE)
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Mac: Cmd + X
                    SEND_STRING(KEY_APPLE_KEY_ACTION(X_X));
                }
                else {
                    // Linux, Windows, etc.: Ctrl + X
                    SEND_STRING(KEY_CTRL_ACTION(X_X));
                }
                #endif
            }
            break;

        case SELALL:
            if (record->event.pressed) {
                #if defined(OS_DETECTION_ENABLE)
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Mac: Cmd+A
                    SEND_STRING(KEY_APPLE_KEY_ACTION(X_A));
                } else {
                    // Windows/Linux: Ctrl+A
                    SEND_STRING(KEY_CTRL_ACTION(X_A));
                }
                #else
                // Default to Windows/Linux if OS detection is not enabled
                SEND_STRING(KEY_CTRL_ACTION(X_A));
                #endif
            }
            break;

        case DELWORD:
            if (record->event.pressed) {
                #if defined(OS_DETECTION_ENABLE)
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Mac: Alt + Backspace
                    tap_code16(LALT(KC_BSPC));
                } else {
                    // Linux, Windows, etc.: Ctrl + Backspace
                    tap_code16(LCTL(KC_BSPC));
                }
                #else
                // Default to Windows/Linux if OS detection is not enabled
                tap_code16(LCTL(KC_BSPC));
                #endif
            }
            break;

        case SELLINE:
            if (record->event.pressed) {
                #if defined(OS_DETECTION_ENABLE)
                os_variant_t host = detected_host_os();
                if (host == OS_MACOS || host == OS_IOS) {
                    // Mac: Cmd+Left, then Cmd+Shift+Right
                    SEND_STRING(SS_LGUI(SS_TAP(X_LEFT)) SS_DELAY(CUSTOM_TAP_DELAY) SS_LGUI(SS_LSFT(SS_TAP(X_RGHT))));
                } else {
                    // Windows/Linux: Home, then Shift+End
                    SEND_STRING(SS_TAP(X_HOME) SS_DELAY(CUSTOM_TAP_DELAY) SS_LSFT(SS_TAP(X_END)));
                }
                #else
                // Default to Windows/Linux if OS detection is not enabled
                SEND_STRING(SS_TAP(X_HOME) SS_DELAY(CUSTOM_TAP_DELAY) SS_LSFT(SS_TAP(X_END)));
                #endif
            }
            break;

        case CKC_UPDIR:
            if (record->event.pressed) {
                SEND_STRING("../");
            }
            break;

        case CKC_SCOPE:
            if (record->event.pressed) {
                SEND_STRING("::");
            }
            break;

        // QMK: shift + backspace = delete
        case KC_BSPC: {
            static uint16_t registered_key = KC_NO;
            if (record->event.pressed) {
                const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
                uint8_t shift_mods = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
#else
                uint8_t shift_mods = mods & MOD_MASK_SHIFT;
#endif
                if (shift_mods) {
                    registered_key = KC_DEL;

                if (shift_mods != MOD_MASK_SHIFT) {
#ifndef NO_ACTION_ONESHOT
                    del_oneshot_mods(MOD_MASK_SHIFT);
#endif
                    unregister_mods(MOD_MASK_SHIFT);
                }
            } else {
                registered_key = KC_BSPC;
            }

            register_code(registered_key);
            set_mods(mods);
        } else {
            unregister_code(registered_key);
        }
      } return false;

    // Other macros...
    }
    return true;
}

void on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        SMTD_LT(CKC_R, KC_R, 2, 1) // SYM layer
        SMTD_LT(CKC_I, KC_I, 2, 1) // SYM layer

        // Custom implementation for MEH+SPACE
        case CKC_SPACE: {
            switch (action) {
                case SMTD_ACTION_TOUCH:
                    break;

                case SMTD_ACTION_TAP:
                    tap_code(KC_SPACE);
                    break;

                case SMTD_ACTION_HOLD:
                    if (tap_count < 2) { // Allow double-tap and hold to be regular space hold
                        // MEH = Ctrl+Shift+Alt
                        register_mods(MOD_LCTL | MOD_LSFT | MOD_LALT);
                    } else {
                        register_code(KC_SPACE);
                    }
                    break;

                case SMTD_ACTION_RELEASE:
                    if (tap_count < 2) {
                        unregister_mods(MOD_LCTL | MOD_LSFT | MOD_LALT);
                    } else {
                        unregister_code(KC_SPACE);
                    }
                    break;
            }
            break;
        }
    }
}
