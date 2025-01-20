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

#ifdef HAVE_NBGL

#include <stdbool.h>  // bool
#include <string.h>   // memset

#include "os.h"
#include "glyphs.h"
#include "os_io_seproxyhal.h"
#include "nbgl_use_case.h"
#include "io.h"
#include "bip32.h"
#include "format.h"

#include "display.h"
#include "constants.h"
#include "globals.h"
#include "sw.h"
#include "validate.h"
#include "menu.h"

// Buffer where the transaction amount string is written
static char g_amount[30];
// Buffer where the transaction address string is written
static char g_address[43];
static char g_address_long[100];

static nbgl_layoutTagValue_t pairs[3];
static nbgl_layoutTagValueList_t pairList;

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

// Public function to start the transaction review
// - Check if the app is in the right state for transaction review
// - Format the amount and address strings in g_amount and g_address buffers
// - Display the first screen of the transaction review
// - Display a warning if the transaction is blind-signed
int ui_display_review(bool is_blind_signed) {
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

    if (is_blind_signed) {
        // Start blind-signing review flow
        nbgl_useCaseReviewBlindSigning(TYPE_TRANSACTION,
                                       &pairList,
                                       &LARGE_ICON,
                                       "Review transaction\nto send NBT",
                                       NULL,
                                       "Sign transaction\nto send NBT",
                                       NULL,
                                       review_choice);
    } else {
        // Start review flow
        nbgl_useCaseReview(TYPE_TRANSACTION,
                           &pairList,
                           &LARGE_ICON,
                           "Review transaction\nto send NBT",
                           NULL,
                           "Sign transaction\nto send NBT",
                           review_choice);
    }

    return 0;
}

// used to simulate several streamed APDUs
static uint8_t streaming_counter;
static char buffer[10];
static bool more_data_to_send() {
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "NBT %u", 10 * streaming_counter);

    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = buffer;
    pairs[1].item = "Address";
    pairs[1].value = "0x1234567890";
    pairs[2].item = "Long text";
    pairs[2].value =
        "Very long text to test the more feature of the screen\n"
        "Very long text to test the more feature of the screen\n"
        "Very long text to test the more feature of the screen\n"
        "Very long text to test the more feature of the screen\n"
        "Very long text to test the more feature of the screen\n"
        "Very long text to test the more feature of the screen\n";

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    // add a 3rd pair with a long text to test the more feature (for the last screen)
    if (streaming_counter == 2) {
        pairList.nbPairs = 3;
    } else {
        pairList.nbPairs = 2;
    }
    pairList.pairs = pairs;
    streaming_counter--;

    // stop streaming when counter is 0 (all APDUs received)
    return streaming_counter > 0;
}

static void onTransactionContinue(bool askMore) {
    if (askMore) {
        if (more_data_to_send()) {
            nbgl_useCaseReviewStreamingContinue(&pairList, onTransactionContinue);
        } else {
            nbgl_useCaseReviewStreamingFinish("Sign transaction\nto send NBT", review_choice);
        }
    } else {
        review_choice(false);
    }
}

int ui_display_streaming_review(bool is_blind_signed) {
    // streaming counter initialization
    streaming_counter = 4;

    if (is_blind_signed) {
        // Start streaming blind-signing review flow
        nbgl_useCaseReviewStreamingBlindSigningStart(TYPE_TRANSACTION,
                                                     &LARGE_ICON,
                                                     "Review transaction\nto send NBT",
                                                     NULL,
                                                     onTransactionContinue);
    } else {
        // Start streaming review flow
        nbgl_useCaseReviewStreamingStart(TYPE_TRANSACTION,
                                         &LARGE_ICON,
                                         "Review transaction\nto send NBT",
                                         NULL,
                                         onTransactionContinue);
    }

    return 0;
}

static void quit_cb(void) {
    io_send_sw(SW_DENY);
    ui_menu_main();
}

static void control_cb(int token, uint8_t index, int page) {
    UNUSED(index);
    UNUSED(page);

    if (token == FIRST_USER_TOKEN) {
        io_send_sw(SW_OK);
    } else {
        io_send_sw(SW_DENY);
    }
    ui_menu_main();
}

int ui_display_generic_review(void) {
    static nbgl_genericContents_t genericContent = {0};
    static nbgl_content_t contentsList[3] = {0};
    uint8_t nbContent = 0;

    // Format amount and address to g_amount and g_address buffers
    memset(g_amount, 0, sizeof(g_amount));
    snprintf(g_amount, sizeof(g_amount), "NBT 0.99");
    memset(g_address, 0, sizeof(g_address));
    snprintf(g_address, sizeof(g_address), "0x1234567890");
    memset(g_address_long, 0, sizeof(g_address_long));
    snprintf(g_address_long,
             sizeof(g_address_long),
             "5A8FgbMkmG2e3J41sBdjvjaBUyz8qHohsQcGtRf63qEUTMBvmA45fpp5pSacMdSg7A3b71RejLzB8EkGbfjp5"
             "PELVHCRUaE");

    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = g_amount;
    pairs[1].item = "Address Short";
    pairs[1].value = g_address;
    pairs[2].item = "Address Long";
    pairs[2].value = g_address_long;

    contentsList[nbContent].type = CENTERED_INFO;
    contentsList[nbContent].content.centeredInfo.text1 = "Centered Info";
    contentsList[nbContent].content.centeredInfo.text2 = "Text 2";
    contentsList[nbContent].content.centeredInfo.icon = &ICON_INFO;
    nbContent++;

    contentsList[nbContent].type = TAG_VALUE_LIST;
    contentsList[nbContent].content.tagValueList.nbMaxLinesForValue = 0;
    contentsList[nbContent].content.tagValueList.nbPairs = 3;
    contentsList[nbContent].content.tagValueList.pairs = pairs;
    nbContent++;

    contentsList[nbContent].type = INFO_BUTTON;
    contentsList[nbContent].content.infoButton.text = "Info Button";
    contentsList[nbContent].content.infoButton.icon = &ICON_APP;
    contentsList[nbContent].content.infoButton.buttonText = "Valid";
    contentsList[nbContent].content.infoButton.buttonToken = FIRST_USER_TOKEN;
    contentsList[nbContent].contentActionCallback = control_cb;
    nbContent++;

    genericContent.contentsList = (const nbgl_content_t *) contentsList;
    genericContent.nbContents = nbContent;

    // Start review flow
    nbgl_useCaseGenericReview(&genericContent, "Cancel", quit_cb);

    return 0;
}

#endif
