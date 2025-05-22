#pragma once

#ifdef SCREEN_SIZE_NANO
#define ICON_APP      C_app_nbgl_tests_14px
#define ICON_HOME     C_home_nbgl_tests_14px
#define ICON_WARNING  C_icon_warning
#define ICON_INFO     C_info_button_14px
#define ICON_BITCOIN  C_bitcoin_14px
#define ICON_SOLANA   C_solana_14px
#define ICON_POLYGON  C_polygon_14px
#define ICON_ETHEREUM C_ethereum_14px
#elif defined(TARGET_STAX) || defined(TARGET_FLEX)
#if (APP_TYPE == APP_DEMO_TYPE)
#define ICON_APP C_app_demo_64px
#else
#define ICON_APP C_app_nbgl_tests_64px
#endif
#define ICON_HOME     ICON_APP
#define ICON_WARNING  C_Warning_64px
#define ICON_INFO     C_info_button_64px
#define ICON_BITCOIN  C_bitcoin_64px
#define ICON_SOLANA   C_solana_64px
#define ICON_POLYGON  C_polygon_64px
#define ICON_ETHEREUM C_ethereum_64px
#elif defined(TARGET_APEX_P)
#if (APP_TYPE == APP_DEMO_TYPE)
#define ICON_APP C_app_demo_48px
#else
#define ICON_APP C_app_nbgl_tests_48px
#endif
#define ICON_HOME     ICON_APP
#define ICON_WARNING  LARGE_WARNING_ICON
#define ICON_INFO     C_info_button_48px
#define ICON_BITCOIN  C_bitcoin_48px
#define ICON_SOLANA   C_solana_48px
#define ICON_POLYGON  C_polygon_48px
#define ICON_ETHEREUM C_ethereum_48px
#endif
