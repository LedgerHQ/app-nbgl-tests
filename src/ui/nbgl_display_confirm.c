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
#include "io.h"
#include "display.h"
#include "status_words.h"

static void callback(void) {
    nbgl_useCaseStatus("Message OK", true, ui_menu_main);
}

int ui_display_confirm() {
    nbgl_useCaseConfirm("Do you confirm", "This message ?", "Confirm", "Cancel", callback);
    io_send_sw(SWO_SUCCESS);
    return 0;
}
