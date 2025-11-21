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
#include "os_io_seproxyhal.h"
#include "nbgl_use_case.h"
#include "io.h"
#include "bip32.h"
#include "format.h"

#include "display.h"
#include "constants.h"
#include "globals.h"
#include "validate.h"
#include "menu.h"

#define PAIRS_NB 5
#ifdef SCREEN_SIZE_WALLET
#define INFO_NB 3
#else
#define INFO_NB 4
#endif

static nbgl_contentTagValue_t pairs[PAIRS_NB] = {0};
static nbgl_contentTagValueList_t pairList = {0};
static nbgl_warning_t warning = {0};
static nbgl_contentValueExt_t extension = {0};
static nbgl_contentInfoList_t infolist = {0};

#ifdef SCREEN_SIZE_WALLET
static const char* const infoTypes[INFO_NB] = {"Smart Contract owner",
                                               "Smart Contract",
                                               "Deployed on"};
#else
static const char* const infoTypes[INFO_NB] = {"Contract owner",
                                               "Contract",
                                               "Contract address",
                                               "Deployed on"};
#endif
static const char* const infoValues[INFO_NB] = {"1inch Network\n1inch.io",
                                                "Aggregation Router V6",
#ifndef SCREEN_SIZE_WALLET
                                                "0x111111125421cA6dc452d289314280a0f8842A65",
#endif
                                                "2024-02-12"};
#ifdef SCREEN_SIZE_WALLET
static const nbgl_contentValueExt_t infoExtensions[INFO_NB] = {
    [1] = {.fullValue = "https://etherscan.io/address/0x111111125421cA6dc452d289314280a0f8842A65",
           .explanation = "Scan to view on Etherscan",
           .title = "0x111111125421cA6dc452d289314280a0f8842A65",
           .aliasType = QR_CODE_ALIAS}};
#endif

// Initialize the pairs array
static void init_pairs(void) {
    explicit_bzero(pairs, sizeof(nbgl_contentTagValue_t) * PAIRS_NB);
    explicit_bzero(&pairList, sizeof(nbgl_contentTagValueList_t));
    pairList.pairs = pairs;
}

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
    uint8_t nbPairs = 0;
    // Setup data to display
    init_pairs();
    pairs[nbPairs].item = "Amount";
    pairs[nbPairs].value = "1.5 BTC";
    nbPairs++;
    pairs[nbPairs].item = "To";
    pairs[nbPairs].value = "bc1ql49ydapnjafl5t2cp9zqpjwe6pdgmxy98859v2";
    nbPairs++;
    pairs[nbPairs].item = "Fees";
    pairs[nbPairs].value = "0.00000698 BTC";
    nbPairs++;

    // Setup list
    pairList.nbPairs = nbPairs;

    // Start review flow
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_BITCOIN,
                       "Review transaction\nto send Bitcoin (demo)",
                       NULL,
#ifdef SCREEN_SIZE_WALLET
                       "Sign transaction to\n send Bitcoin? (demo)",
#else
                       NULL,
#endif
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
    uint8_t nbPairs = 0;
    // Setup data to display
    init_pairs();
    pairs[nbPairs].item = "From";
    pairs[nbPairs].value = "0x519192a437e6aeb895Cec72828A73B11b698dE3a";
    nbPairs++;
    pairs[nbPairs].item = "Amount";
    pairs[nbPairs].value = "ETH 0";
    nbPairs++;
    pairs[nbPairs].item = "To";
    pairs[nbPairs].value = "0x16D5A408e807db8eF7c578279BEeEe6b228f1c1C";
    nbPairs++;
    pairs[nbPairs].item = "Max fees";
    pairs[nbPairs].value = "ETH 0.0047303";
    nbPairs++;

    // Setup list
    pairList.nbPairs = nbPairs;

    // Start blind-signing review flow
    nbgl_useCaseReviewBlindSigning(TYPE_TRANSACTION,
                                   &pairList,
                                   &ICON_ETHEREUM,
                                   "Review transaction\n(demo)",
                                   NULL,
#ifdef SCREEN_SIZE_WALLET
                                   "Accept risk and sign\ntransaction? (demo)",
#else
                                   "Accept risk and sign transaction",
#endif
                                   NULL,
                                   review_choice);

    return 0;
}

#ifdef SCREEN_SIZE_NANO
static const nbgl_contentCenter_t warningInfo = {.icon = &WARNING_ICON,
                                                 .title = "Ledger Demo",
                                                 .description = "This Demo is\npotentially risky."};

// Details page shown when the user taps the top-right icon.
static const nbgl_warningDetails_t warningDetails = {
    .title = "Blind signing ahead",
    .type = CENTERED_INFO_WARNING,
    .centeredInfo.illustrType = ICON_ILLUSTRATION,
    .centeredInfo.icon = &WARNING_ICON,
    .centeredInfo.title = "Blind signing ahead",
    .centeredInfo.description = "You could loose all your assets."};
#endif
int ui_display_review_with_warning() {
    uint8_t nbPairs = 0;
    // Setup data to display
    init_pairs();
    pairs[nbPairs].item = "From";
    pairs[nbPairs].value = "0x519192a437e6aeb895Cec72828A73B11b698dE3a";
    nbPairs++;
    pairs[nbPairs].item = "Amount";
    pairs[nbPairs].value = "ETH 0";
    nbPairs++;
    pairs[nbPairs].item = "To";
    pairs[nbPairs].value = "0x16D5A408e807db8eF7c578279BEeEe6b228f1c1C";
    nbPairs++;
    pairs[nbPairs].item = "Max fees";
    pairs[nbPairs].value = "ETH 0.0047303";
    nbPairs++;

    // Setup list
    pairList.nbPairs = nbPairs;

    // Setup warning
    explicit_bzero(&warning, sizeof(nbgl_warning_t));
#ifdef SCREEN_SIZE_WALLET
    warning.predefinedSet = (1 << W3C_RISK_DETECTED_WARN);
    warning.reportProvider = "Ledger Demo";
    warning.providerMessage = "This Demo is\npotentially risky.";
    warning.reportUrl = "ledger.com";
#else
    warning.introDetails = &warningDetails;
    warning.info = &warningInfo;
#endif

    // Start review flow
    nbgl_useCaseAdvancedReview(TYPE_TRANSACTION,
                               &pairList,
                               &ICON_ETHEREUM,
                               "Review transaction\n(demo)",
                               NULL,
#ifdef SCREEN_SIZE_WALLET
                               "Accept risk and sign\ntransaction? (demo)",
#else
                               "Accept risk and sign transaction",
#endif
                               NULL,
                               &warning,
                               review_choice);

    return 0;
}

int ui_display_swap_review() {
    uint8_t nbPairs = 0;
    init_pairs();
    explicit_bzero(&extension, sizeof(nbgl_contentValueExt_t));
    explicit_bzero(&infolist, sizeof(nbgl_contentInfoList_t));
    // Setup data to display
    pairs[nbPairs].item = "Interaction with";
    pairs[nbPairs].value = "1inch";
    pairs[nbPairs].extension = &extension;
    pairs[nbPairs].aliasValue = 1;
    extension.aliasType = INFO_LIST_ALIAS;
    extension.infolist = &infolist;
    infolist.nbInfos = INFO_NB;
    infolist.infoTypes = infoTypes;
    infolist.infoContents = infoValues;
#ifdef SCREEN_SIZE_WALLET
    infolist.infoExtensions = infoExtensions;
    infolist.withExtensions = true;
#endif
    nbPairs++;
    pairs[nbPairs].item = "Send";
    pairs[nbPairs].value = "USDT 42";
    nbPairs++;
    pairs[nbPairs].item = "Receive minimum";
    pairs[nbPairs].value = "SUSHI 54.66";
    nbPairs++;
    pairs[nbPairs].item = "Max fees";
    pairs[nbPairs].value = "POL 0.008273";
    nbPairs++;
    pairs[nbPairs].item = "Network";
    pairs[nbPairs].value = "Polygon";
    nbPairs++;

    // Setup list
    pairList.nbPairs = nbPairs;

    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_POLYGON,
                       "Review transaction to\nswap tokens (demo)",
                       NULL,
#ifdef SCREEN_SIZE_WALLET
                       "Sign transaction to\nswap tokens? (demo)",
#else
                       NULL,
#endif
                       review_choice);

    return 0;
}
