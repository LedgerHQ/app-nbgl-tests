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
// Buffer where the transaction fees string is written
static char g_fees[40];

static nbgl_layoutTagValue_t pairs[4];
static nbgl_layoutTagValueList_t pairList;

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
    // Format amount and address to g_amount and g_address buffers
    memset(g_amount, 0, sizeof(g_amount));
    snprintf(g_amount, sizeof(g_amount), "1.5 BTC");
    memset(g_address, 0, sizeof(g_address));
    snprintf(g_address, sizeof(g_address), "bc1ql49ydapnjafl5t2cp9zqpjwe6pdgmxy98859v2");
    memset(g_fees, 0, sizeof(g_fees));
    snprintf(g_fees, sizeof(g_fees), "0.00000698 BTC");

    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = g_amount;
    pairs[1].item = "To";
    pairs[1].value = g_address;
    pairs[2].item = "Fees";
    pairs[2].value = g_fees;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 3;
    pairList.pairs = pairs;

    // Start review flow
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &C_bitcoin_64px,
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
                              &C_solana_64px,
                              "Verify Solana address\n(demo)",
                              NULL,
                              review_choice_address_SOL);
    return 0;
}

int ui_display_BS_staking_review() {
    memset(g_amount, 0, sizeof(g_amount));
    snprintf(g_amount, sizeof(g_amount), "0 ETH");
    memset(g_address, 0, sizeof(g_address));
    snprintf(g_address, sizeof(g_address), "0x51917F958D2ee523a2206206994597C13D83de3a");
    memset(g_fees, 0, sizeof(g_fees));
    snprintf(g_fees, sizeof(g_fees), "0.0047303 ETH");

    // Setup data to display
    pairs[0].item = "Amount";
    pairs[0].value = g_amount;
    pairs[1].item = "Address";
    pairs[1].value = g_address;
    pairs[2].item = "Max fees";
    pairs[2].value = g_fees;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = 3;
    pairList.pairs = pairs;

    // Start blind-signing review flow
    nbgl_useCaseReviewBlindSigning(TYPE_TRANSACTION,
                                   &pairList,
                                   &C_ethereum_64px,
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

int ui_display_swap_review() {
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

    nbgl_tipBox_t tipBox = {.icon = &C_info_button_64px,
                            .text = "You're interacting with a contract from:\n1inch",
                            .modalTitle = "Contract information",
                            .infos.nbInfos = INFO_NB,
                            .infos.infoTypes = infoTypes,
                            .infos.infoContents = infoValues,
                            .infos.infoExtensions = infoExtensions,
                            .infos.withExtensions = true,
                            .type = INFOS_LIST};

    nbgl_useCaseAdvancedReview(TYPE_TRANSACTION,
                               &pairList,
                               &C_polygon_64px,
                               "Review transaction to\nswap tokens (demo)",
                               NULL,
                               "Sign transaction to\nswap tokens? (demo)",
                               &tipBox,
                               review_choice);

    return 0;
}

#endif
