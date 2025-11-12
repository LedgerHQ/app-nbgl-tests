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

#include <stdbool.h>  // bool
#include <string.h>   // memset

#include "os.h"
#include "glyphs.h"
#include "nbgl_use_case.h"

#include "globals.h"
#include "status_words.h"
#include "validate.h"
#include "display.h"
#include "menu.h"

// Following code is expected to be deprecated and removed.
// It is kept for compatibility with the previous version of the app.
// but not ported on Nano

// Buffer where the transaction amount string is written
static char g_amount[30];
// Buffer where the transaction address string is written
static char g_address[43];

static nbgl_contentTagValue_t pairs[3];
static nbgl_contentTagValueList_t pairList;

// called when long press button on 3rd page is long-touched or when reject footer is touched
static void review_choice(bool confirm) {
    // Answer, display a status page and go back to main
    validate_transaction(confirm);
    if (confirm) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_menu_main);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
    }
}

#ifdef SCREEN_SIZE_WALLET
static nbgl_pageInfoLongPress_t infoLongPress;

static void reject_callback(void) {
    review_choice(false);
}

static bool light_review = false;

void continue_callback() {
    // Format amount and address to g_amount and g_address buffers
    memset(g_amount, 0, sizeof(g_amount));
    snprintf(g_amount, sizeof(g_amount), "NBT 0.99");
    memset(g_address, 0, sizeof(g_address));
    snprintf(g_address, sizeof(g_address), "0x1234567890");

    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = g_amount;
    pairs[1].item = "Address";
    pairs[1].value = g_address;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 2;
    pairList.pairs = pairs;

    infoLongPress.icon = &ICON_APP;
    infoLongPress.longPressText = light_review ? "Confirm" : "Hold to sign";
    infoLongPress.longPressToken = 0;
    infoLongPress.tuneId = TUNE_TAP_CASUAL;
    infoLongPress.text = "Sign transaction\nto send NBT";

    if (light_review) {
        nbgl_useCaseStaticReviewLight(&pairList, &infoLongPress, "Reject", review_choice);
    } else {
        nbgl_useCaseStaticReview(&pairList, &infoLongPress, "Reject", review_choice);
    }
}

// start a static review flow
int ui_display_static_review(bool light) {
    light_review = light;
    nbgl_useCaseReviewStart(&ICON_APP,
                            "Review transaction\nto send NBT",
                            NULL,
                            "Reject",
                            continue_callback,
                            reject_callback);

    return 0;
}
#else   // SCREEN_SIZE_WALLET
int ui_display_static_review(bool light) {
    UNUSED(light);
    io_send_sw(SWO_INVALID_INS);
    return 0;
}
#endif  // SCREEN_SIZE_WALLET

int ui_display_light_review() {
    // Format amount and address to g_amount and g_address buffers
    memset(g_amount, 0, sizeof(g_amount));
    snprintf(g_amount, sizeof(g_amount), "NBT 0.99");
    memset(g_address, 0, sizeof(g_address));
    snprintf(g_address, sizeof(g_address), "0x1234567890");

    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = g_amount;
    pairs[1].item = "Address";
    pairs[1].value = g_address;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 2;
    pairList.pairs = pairs;

    // Start light review flow
    nbgl_useCaseReviewLight(TYPE_TRANSACTION,
                            &pairList,
                            &ICON_APP,
                            "Review transaction\nto send NBT",
                            NULL,
#ifdef SCREEN_SIZE_WALLET
                            "Sign transaction\nto send NBT",
#else
                            NULL,
#endif
                            review_choice);
    return 0;
}
