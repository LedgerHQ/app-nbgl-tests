#pragma once

#ifdef SCREEN_SIZE_NANO
#define LARGE_ICON         C_app_nbgl_tests_16px
#define LARGE_WARNING_ICON C_icon_warning
#else
#define LARGE_ICON         C_app_nbgl_tests_64px
#define LARGE_WARNING_ICON C_Warning_64px
#endif

/**
 * Show main menu (ready screen, version, about, quit).
 */
void ui_menu_main(void);

/**
 * Show about submenu (copyright, date).
 */
void ui_menu_about(void);

int ui_display_review(bool is_blind_signed);
int ui_display_address_review();
int ui_display_long_address_review();
int ui_display_streaming_review(bool is_blind_signed);
int ui_display_long_address_review_with_tags();
int ui_display_spinner();
int ui_display_static_review();
int ui_display_light_review();
