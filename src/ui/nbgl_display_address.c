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

static void review_choice(bool confirm) {
    // Answer, display a status page and go back to main
    validate_address(confirm);
    if (confirm) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_VERIFIED, ui_menu_main);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_REJECTED, ui_menu_main);
    }
}

int ui_display_address_review() {
    nbgl_useCaseAddressReview("0xABCDEF1234",
                              NULL,
                              &LARGE_ICON,
                              "Verify NBT address",
                              NULL,
                              review_choice);
    return 0;
}

int ui_display_long_address_review() {
    nbgl_useCaseAddressReview(
        "5A8FgbMkmG2e3J41sBdjvjaBUyz8qHohsQcGtRf63qEUTMBvmA45fp"
        "p5pSacMdSg7A3b71RejLzB8EkGbfjp5PELVHCRUaE",
        NULL,
        &LARGE_ICON,
        "Verify NBT address",
        NULL,
        review_choice);
    return 0;
}

// 2 pairs of tag/value to display in second page
static nbgl_layoutTagValue_t pairs[2];
static nbgl_layoutTagValueList_t pairList;

int ui_display_long_address_review_with_tags() {
    // Setup data to display
    pairs[0].item = "Type address";
    pairs[0].value = "dummy type";
    pairs[1].item = "Sub address";
    pairs[1].value = "dummy sub address";

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 2;
    pairList.pairs = pairs;

    nbgl_useCaseAddressReview(
        "5A8FgbMkmG2e3J41sBdjvjaBUyz8qHohsQcGtRf63qEUTMBvmA45fp"
        "p5pSacMdSg7A3b71RejLzB8EkGbfjp5PELVHCRUaE",
        &pairList,
        &LARGE_ICON,
        "Verify NBT address",
        NULL,
        review_choice);
    return 0;
}

#endif
