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

static nbgl_contentTagValue_t pairs[4];
static nbgl_contentTagValueList_t pairList;

// called when long press button on 3rd page is long-touched or when reject footer is touched
static void review_choice(bool confirm) {
    // Answer, display a status page and go back to main
    validate_transaction(confirm);
    if (confirm) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_display_demo_list);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_display_demo_list);
    }
}

int ui_display_BTC_review() {
    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = "1.5 BTC";
    pairs[1].item = "To";
    pairs[1].value = "bc1ql49ydapnjafl5t2cp9zqpjwe6pdgmxy98859v2";
    pairs[2].item = "Fees";
    pairs[2].value = "0.00000698 BTC";

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 3;
    pairList.pairs = pairs;

    // Start review flow
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_BITCOIN,
                       "Review transaction\nto send Bitcoin (demo)",
                       NULL,
                       "Sign transaction to\n send Bitcoin? (demo)",
                       review_choice);

    return 0;
}

static void review_choice_address_SOL(bool confirm) {
    // Answer, display a status page and go back to main
    validate_address(confirm);
    if (confirm) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_VERIFIED, ui_display_demo_list);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_REJECTED, ui_display_demo_list);
    }
}

int ui_display_SOL_address_review() {
    nbgl_useCaseAddressReview("7EcDhSYGxXyscszYEp35KHN8vvw3svAuLKTzXwCFLtV",
                              NULL,
                              &ICON_SOLANA,
                              "Verify Solana address\n(demo)",
                              NULL,
                              review_choice_address_SOL);
    return 0;
}

int ui_display_BS_staking_review() {
    // Setup data to display
    pairs[0].item = "From";
    pairs[0].value = "0x519192a437e6aeb895Cec72828A73B11b698dE3a";
    pairs[1].item = "Amount";
    pairs[1].value = "ETH 0";
    pairs[2].item = "To";
    pairs[2].value = "0x16D5A408e807db8eF7c578279BEeEe6b228f1c1C";
    pairs[3].item = "Max fees";
    pairs[3].value = "ETH 0.0047303";

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 4;
    pairList.pairs = pairs;

    // Start blind-signing review flow
    nbgl_useCaseReviewBlindSigning(TYPE_TRANSACTION,
                                   &pairList,
                                   &ICON_ETHEREUM,
                                   "Review transaction\n(demo)",
                                   NULL,
                                   "Accept risk and sign\ntransaction? (demo)",
                                   NULL,
                                   review_choice);

    return 0;
}

#define INFO_NB 3
static const char* const infoTypes[INFO_NB] = {"Contract owner", "Contract", "Deployed on"};
static const char* const infoValues[INFO_NB] = {"1inch Network\n1inch.io",
                                                "Aggregation Router V6",
                                                "2024-02-12"};
static const nbgl_contentValueExt_t infoExtensions[INFO_NB] = {
    [1] = {.fullValue = "https://etherscan.io/address/0x111111125421cA6dc452d289314280a0f8842A65",
           .explanation = "Scan to view on Etherscan",
           .title = "0x111111125421cA6dc452d289314280a0f8842A65",
           .aliasType = QR_CODE_ALIAS}};

static nbgl_tipBox_t tipBox;

int ui_display_swap_review() {
    explicit_bzero(&tipBox, sizeof(tipBox));
    // Setup data to display
    pairs[0].item = "Send";
    pairs[0].value = "USDT 42";
    pairs[1].item = "Receive minimum";
    pairs[1].value = "SUSHI 54.66";
    pairs[2].item = "Max fees";
    pairs[2].value = "POL 0.008273";
    pairs[3].item = "Network";
    pairs[3].value = "Polygon";

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 4;
    pairList.pairs = pairs;

    tipBox.icon = &ICON_INFO;
    tipBox.text = "Interaction with a\nsmart contract from:\n1inch";
    tipBox.modalTitle = "Contract information";
    tipBox.infos.nbInfos = INFO_NB;
    tipBox.infos.infoTypes = infoTypes;
    tipBox.infos.infoContents = infoValues;
    tipBox.infos.infoExtensions = infoExtensions;
    tipBox.infos.withExtensions = true;
    tipBox.type = INFOS_LIST;

    nbgl_useCaseAdvancedReview(TYPE_TRANSACTION,
                               &pairList,
                               &ICON_POLYGON,
                               "Review transaction to\nswap tokens (demo)",
                               NULL,
                               "Sign transaction to\nswap tokens? (demo)",
                               &tipBox,
                               NULL,
                               review_choice);

    return 0;
}

#endif
