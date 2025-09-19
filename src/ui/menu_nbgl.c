
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

#include "os.h"
#include "glyphs.h"
#include "nbgl_use_case.h"
#include "display.h"

#include "globals.h"
#include "menu.h"
#include "sw.h"

extern void app_exit(void);

//  -----------------------------------------------------------
//  --------------- NBGL TEST HOME PAGE -----------------------
//  -----------------------------------------------------------

#define SETTING_INFO_NB 4
static const char* const INFO_TYPES[SETTING_INFO_NB] = {"Version",
                                                        "Developer",
                                                        "Dummy 1",
                                                        "Dummy 2"};

static const char* const INFO_CONTENTS[SETTING_INFO_NB] = {APPVERSION,
                                                           "Ledger",
                                                           "Dummy 1 information",
                                                           "Dummy 2 information"};

// settings switches definitions
enum { DUMMY_SWITCH_1_TOKEN = FIRST_USER_TOKEN, DUMMY_SWITCH_2_TOKEN };
enum { DUMMY_SWITCH_1_ID = 0, DUMMY_SWITCH_2_ID, SETTINGS_SWITCHES_NB };

static nbgl_contentSwitch_t switches[SETTINGS_SWITCHES_NB] = {0};

static const nbgl_contentInfoList_t infoList = {
    .nbInfos = SETTING_INFO_NB,
    .infoTypes = INFO_TYPES,
    .infoContents = INFO_CONTENTS,
};

static uint8_t initSettingPage;
static nbgl_homeAction_t homeAction;
static void review_warning_choice(bool confirm);
static void controls_callback(int token, uint8_t index, int page);

// settings menu definition
#define SETTING_CONTENTS_NB 1
static const nbgl_content_t contents[SETTING_CONTENTS_NB] = {
    {.type = SWITCHES_LIST,
     .content.switchesList.nbSwitches = SETTINGS_SWITCHES_NB,
     .content.switchesList.switches = switches,
     .contentActionCallback = controls_callback}};

static const nbgl_genericContents_t settingContents = {.callbackCallNeeded = false,
                                                       .contentsList = contents,
                                                       .nbContents = SETTING_CONTENTS_NB};

// callback for setting warning choice
static void review_warning_choice(bool confirm) {
    uint8_t switch_value;
    if (confirm) {
        // toggle the switch value
        switch_value = !N_storage.dummy2_allowed;
        switches[DUMMY_SWITCH_2_ID].initState = (nbgl_state_t) switch_value;
        // store the new setting value in NVM
        nvm_write((void*) &N_storage.dummy2_allowed, &switch_value, 1);
    }

    // Reset setting menu to the right page
    nbgl_useCaseHomeAndSettings(APPNAME,
                                &ICON_HOME,
                                NULL,
                                initSettingPage,
                                &settingContents,
                                &infoList,
                                &homeAction,
                                app_exit);
}

static void controls_callback(int token, uint8_t index, int page) {
    UNUSED(index);

    initSettingPage = page;

    uint8_t switch_value;
    if (token == DUMMY_SWITCH_1_TOKEN) {
        // Dummy 1 switch touched
        // toggle the switch value
        switch_value = !N_storage.dummy1_allowed;
        switches[DUMMY_SWITCH_1_ID].initState = (nbgl_state_t) switch_value;
        // store the new setting value in NVM
        nvm_write((void*) &N_storage.dummy1_allowed, &switch_value, 1);
    } else if (token == DUMMY_SWITCH_2_TOKEN) {
        // Dummy 2 switch touched

        // in this example we display a warning when the user wants
        // to activate the dummy 2 setting
        if (!N_storage.dummy2_allowed) {
            // Display the warning message and ask the user to confirm
            nbgl_useCaseChoice(&ICON_WARNING,
                               "Dummy 2",
                               "Are you sure to\nallow dummy 2\nin transactions?",
                               "I understand, confirm",
                               "Cancel",
                               review_warning_choice);
        } else {
            // toggle the switch value
            switch_value = !N_storage.dummy2_allowed;
            switches[DUMMY_SWITCH_2_ID].initState = (nbgl_state_t) switch_value;
            // store the new setting value in NVM
            nvm_write((void*) &N_storage.dummy2_allowed, &switch_value, 1);
        }
    }
}

// home page definition
void ui_menu_main_nbgl_test(void) {
    // Initialize switches data
    switches[DUMMY_SWITCH_1_ID].initState = (nbgl_state_t) N_storage.dummy1_allowed;
    switches[DUMMY_SWITCH_1_ID].text = "Dummy 1";
    switches[DUMMY_SWITCH_1_ID].subText = "Allow dummy 1\nin transactions";
    switches[DUMMY_SWITCH_1_ID].token = DUMMY_SWITCH_1_TOKEN;
#ifdef HAVE_PIEZO_SOUND
    switches[DUMMY_SWITCH_1_ID].tuneId = TUNE_TAP_CASUAL;
#endif

    switches[DUMMY_SWITCH_2_ID].initState = (nbgl_state_t) N_storage.dummy2_allowed;
    switches[DUMMY_SWITCH_2_ID].text = "Dummy 2";
    switches[DUMMY_SWITCH_2_ID].subText = "Allow dummy 2\nin transactions";
    switches[DUMMY_SWITCH_2_ID].token = DUMMY_SWITCH_2_TOKEN;
#ifdef HAVE_PIEZO_SOUND
    switches[DUMMY_SWITCH_2_ID].tuneId = TUNE_TAP_CASUAL;
#endif

    homeAction.callback = (nbgl_callback_t) ui_display_demo_list;
    homeAction.icon = NULL;
    homeAction.text = "Display flows";
    nbgl_useCaseHomeAndSettings(APPNAME,
                                &ICON_HOME,
                                NULL,
                                INIT_HOME_PAGE,
                                &settingContents,
                                &infoList,
                                &homeAction,
                                app_exit);
}

//  -----------------------------------------------------------
//  -------------------- DEMO HOME PAGE -----------------------
//  -----------------------------------------------------------

#define SETTING_DEMO_INFO_NB 2
static const char* const INFO_TYPES_DEMO[SETTING_DEMO_INFO_NB] = {"Version", "Developer"};

static const char* const INFO_CONTENTS_DEMO[SETTING_DEMO_INFO_NB] = {APPVERSION, "Ledger"};

static const nbgl_contentInfoList_t infoListDemo = {
    .nbInfos = SETTING_DEMO_INFO_NB,
    .infoTypes = INFO_TYPES_DEMO,
    .infoContents = INFO_CONTENTS_DEMO,
};

static nbgl_homeAction_t homeActionDemo;
// home page definition
void ui_menu_main_demo(void) {
    homeActionDemo.callback = (nbgl_callback_t) ui_display_demo_list;
    homeActionDemo.icon = NULL;
    homeActionDemo.text = "View demos";
    nbgl_useCaseHomeAndSettings(APPNAME,
                                &ICON_HOME,
                                "Showcase transactions and\n"
                                "address verification, without\n"
                                "spending.",
                                INIT_HOME_PAGE,
                                NULL,
                                &infoListDemo,
                                &homeActionDemo,
                                app_exit);
}

//  -----------------------------------------------------------
//  -------------------- DEMO FLOW LIST -----------------------
//  -----------------------------------------------------------

// demo flow
#define DEMO_FLOW_NB 5

enum {
    BTC_SEND_REVIEW_TOKEN = FIRST_USER_TOKEN,
    SWAP_1INCH_REVIEW_TOKEN,
    STAKE_BLIND_SIGNING_REVIEW_TOKEN,
    SOL_ADDRESS_REVIEW_TOKEN,
    ETH_WARNING_REVIEW_TOKEN
};

static const char* const barTexts[DEMO_FLOW_NB] = {"Send bitcoin",
                                                   "Swap with 1inch",
                                                   "Blind-sign on Ethereum",
                                                   "Receive SOL",
                                                   "Warning with Ethereum"};

static const uint8_t tokens[DEMO_FLOW_NB] = {BTC_SEND_REVIEW_TOKEN,
                                             SWAP_1INCH_REVIEW_TOKEN,
                                             STAKE_BLIND_SIGNING_REVIEW_TOKEN,
                                             SOL_ADDRESS_REVIEW_TOKEN,
                                             ETH_WARNING_REVIEW_TOKEN};

static void demo_control_cb(int token, uint8_t index) {
    UNUSED(index);
    switch (token) {
        case BTC_SEND_REVIEW_TOKEN:
            ui_display_BTC_review();
            break;
        case SWAP_1INCH_REVIEW_TOKEN:
            ui_display_swap_review();
            break;
        case STAKE_BLIND_SIGNING_REVIEW_TOKEN:
            ui_display_BS_staking_review();
            break;
        case SOL_ADDRESS_REVIEW_TOKEN:
            ui_display_SOL_address_review();
            break;
        case ETH_WARNING_REVIEW_TOKEN:
            ui_display_review_with_warning();
            break;
        default:
            PRINTF("Should not happen !");
            break;
    }
}

static bool nav_bar_cb(uint8_t page, nbgl_pageContent_t* content) {
    UNUSED(page);
#ifdef HAVE_PIEZO_SOUND
    content->tuneId = NBGL_NO_TUNE;
#endif
    content->type = BARS_LIST;
    content->barsList.barTexts = barTexts;
    content->barsList.tokens = tokens;
    content->barsList.nbBars = DEMO_FLOW_NB;
#ifdef HAVE_PIEZO_SOUND
    content->barsList.tuneId = TUNE_TAP_CASUAL;
#endif
    return true;
}

static bool nav_switch_cb(uint8_t page, nbgl_pageContent_t* content) {
    UNUSED(page);
    content->type = SWITCHES_LIST;
    content->switchesList.nbSwitches = SETTINGS_SWITCHES_NB;
    content->switchesList.switches = switches;
    return true;
}

#define CHOICES_NB 4
static const char* const choicesTexts[CHOICES_NB] = {"Choice 1",
                                                     "Choice 2",
                                                     "Choice 3",
                                                     "Choice 4"};

static bool nav_choice_cb(uint8_t page, nbgl_pageContent_t* content) {
    UNUSED(page);
    content->type = CHOICES_LIST;
    content->choicesList.names = choicesTexts;
    content->choicesList.token = FIRST_USER_TOKEN;
    content->choicesList.initChoice = 0;
    content->choicesList.nbChoices = CHOICES_NB;
    return true;
}

static bool nav_button_cb(uint8_t page, nbgl_pageContent_t* content) {
    UNUSED(page);
    content->type = INFO_BUTTON;
    content->infoButton.text = "Test info button";
    content->infoButton.icon = &ICON_APP;
    content->infoButton.buttonText = "Press me";
    content->infoButton.buttonToken = 23;
    return true;
}

static bool nav_info_cb(uint8_t page, nbgl_pageContent_t* content) {
    UNUSED(page);
    content->type = CENTERED_INFO;
    content->centeredInfo.text1 = "Test centered info";
    content->centeredInfo.icon = &ICON_APP;
    content->centeredInfo.text2 = "example text";
    return true;
}

// display the list of demo flows
void ui_display_demo_list(void) {
    nbgl_useCaseNavigableContent("Select demo", 0, 1, ui_menu_main, nav_bar_cb, demo_control_cb);
}

void ui_menu_main(void) {
    if (APP_TYPE == APP_DEMO_TYPE) {
        ui_menu_main_demo();
    } else {
        ui_menu_main_nbgl_test();
    }
}

static void quit_cb(void) {
    io_send_sw(SW_OK);
    ui_menu_main();
}

int ui_display_generic_config() {
    nbgl_useCaseGenericConfiguration("Generic Config", 0, &settingContents, quit_cb);
    return 0;
}

int ui_display_generic_settings() {
    nbgl_useCaseGenericSettings(APPNAME, 0, &settingContents, &infoList, quit_cb);
    return 0;
}

static void nav_control_cb(int token, uint8_t index) {
    UNUSED(token);
    UNUSED(index);
}

int ui_display_navigation(uint8_t nav_type) {
    uint16_t sw = SW_OK;
    switch (nav_type) {
        case P1_NAV_CONTENT_CENTERED_INFO:
            nbgl_useCaseNavigableContent("Centered Info",
                                         0,
                                         1,
                                         ui_menu_main,
                                         nav_info_cb,
                                         nav_control_cb);
            break;
        case P1_NAV_CONTENT_INFO_BUTTON:
            nbgl_useCaseNavigableContent("Info Button",
                                         0,
                                         1,
                                         ui_menu_main,
                                         nav_button_cb,
                                         nav_control_cb);
            break;
        case P1_NAV_CONTENT_SWITCHES:
            nbgl_useCaseNavigableContent("Switches List",
                                         0,
                                         1,
                                         ui_menu_main,
                                         nav_switch_cb,
                                         nav_control_cb);
            break;
        case P1_NAV_CONTENT_CHOICES:
            nbgl_useCaseNavigableContent("Choices List",
                                         0,
                                         1,
                                         ui_menu_main,
                                         nav_choice_cb,
                                         nav_control_cb);
            break;
        case P1_NAV_CONTENT_BARS:
            nbgl_useCaseNavigableContent("Bars List",
                                         0,
                                         1,
                                         ui_menu_main,
                                         nav_bar_cb,
                                         nav_control_cb);
            break;
        default:
            sw = SW_WRONG_P1P2;
            break;
    }
    io_send_sw(sw);
    return 0;
}
