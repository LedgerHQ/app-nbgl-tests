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

#include <stdint.h>
#include <stdbool.h>

#include "buffer.h"
#include "io.h"
#include "ledger_assert.h"

#include "dispatcher.h"
#include "constants.h"
#include "globals.h"
#include "types.h"
#include "sw.h"
#include "get_version.h"
#include "get_app_name.h"
#include "display.h"

int apdu_dispatcher(const command_t *cmd) {
    LEDGER_ASSERT(cmd != NULL, "NULL cmd");

    if (cmd->cla != CLA) {
        return io_send_sw(SW_CLA_NOT_SUPPORTED);
    }

    switch (cmd->ins) {
        case GET_VERSION:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_version();
        case GET_APP_NAME:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_app_name();
        case TEST_SPINNER:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_spinner();
        case TEST_USE_CASE_REVIEW:
            // P1 & P2 may be used later as test/sub-test number
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_review(false);
        case TEST_USE_CASE_REVIEW_BLIND_SIGNING:
            // P1 & P2 may be used later as test/sub-test number
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_review(true);
        case TEST_USE_CASE_ADDRESS_REVIEW:
            if (cmd->p1 == 0) {
                return ui_display_address_review();
            } else if (cmd->p1 == 1) {
                return ui_display_long_address_review();
            } else if (cmd->p1 == 2) {
                return ui_display_long_address_review_with_tags();
            }
            return io_send_sw(SW_WRONG_P1P2);
        case TEST_USE_CASE_STREAMING_REVIEW:
            // P1 & P2 may be used later as test/sub-test number
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_streaming_review(false);

        case TEST_USE_CASE_STREAMING_REVIEW_BLIND_SIGNING:
            // P1 & P2 may be used later as test/sub-test number
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_streaming_review(true);
        case TEST_USE_CASE_STATIC_REVIEW:
            // P1 & P2 may be used later as test/sub-test number
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_static_review();
        case TEST_USE_CASE_LIGHT_REVIEW:
            // P1 & P2 may be used later as test/sub-test number
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }
            return ui_display_light_review();
        default:
            return io_send_sw(SW_INS_NOT_SUPPORTED);
    }
}
