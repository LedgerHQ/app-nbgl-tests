#pragma once

#include <stdbool.h>  // bool

/**
 * Show main menu (ready screen, version, about, quit).
 */
void ui_menu_main(void);
int ui_display_review(bool is_blind_signed);
int ui_display_address_review();
int ui_display_long_address_review();
int ui_display_streaming_review(bool is_blind_signed);
int ui_display_long_address_review_with_tags();
int ui_display_spinner();
int ui_display_static_review();
int ui_display_light_review();
