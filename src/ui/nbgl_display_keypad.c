/*****************************************************************************
 *   Ledger App NBGL_Tests.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include "nbgl_use_case.h"
#include "os.h"
#include "io.h"
#include "globals.h"
#include "display.h"
#include "sw.h"

#define PIN_LEN 4
#define PIN_VAL "5555"

#ifdef NBGL_KEYPAD

static void validate_cb(const uint8_t* entry, uint8_t length) {
    bool isSuccess = (length == PIN_LEN) && (memcmp(entry, PIN_VAL, PIN_LEN) == 0);
    io_send_sw(isSuccess ? SW_OK : SW_BAD_STATE);
    nbgl_useCaseStatus("Pin Status", isSuccess, ui_menu_main);
}

static void quit_cb(void) {
    io_send_sw(SW_OK);
    ui_menu_main();
}

int ui_display_keypad_digits() {
    nbgl_useCaseKeypad("Digit Keypad (5555)", 4, 4, false, false, validate_cb, quit_cb);
    return 0;
}

int ui_display_keypad_pin() {
    nbgl_useCaseKeypad("PIN Keypad (5555)", 4, 4, false, true, validate_cb, quit_cb);
    return 0;
}
#else   // NBGL_KEYPAD
int ui_display_keypad_digits() {
    io_send_sw(SW_INS_NOT_SUPPORTED);
    return 0;
}

int ui_display_keypad_pin() {
    io_send_sw(SW_INS_NOT_SUPPORTED);
    return 0;
}
#endif  // NBGL_KEYPAD
