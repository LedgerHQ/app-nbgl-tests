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

#include "os_io_seproxyhal.h"
#include "io.h"
#include "sw.h"

#ifdef HAVE_PIEZO_SOUND

int ui_play_sound(uint8_t tune_type) {
    if (tune_type == 0 || tune_type > NB_TUNES) {
        return io_send_sw(SW_WRONG_P1P2);
    }
    io_seproxyhal_play_tune(tune_type);
    io_send_sw(SW_OK);
    return 0;
}

#else  // HAVE_PIEZO_SOUND

int ui_play_sound() {
    io_send_sw(SW_INS_NOT_SUPPORTED);
    return 0;
}

#endif  // HAVE_PIEZO_SOUND
