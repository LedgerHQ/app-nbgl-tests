#pragma once

#include <stdbool.h>  // bool

/**
 * Show main menu (ready screen, version, about, quit).
 */
void ui_menu_main();
void ui_display_demo_list();
int ui_display_review(bool is_blind_signed);
int ui_display_address_review();
int ui_display_long_address_review();
int ui_display_streaming_review(bool is_blind_signed);
int ui_display_long_address_review_with_tags();
int ui_display_spinner();
int ui_display_static_review(bool light);
int ui_display_light_review();
int ui_display_BTC_review();
int ui_display_SOL_address_review();
int ui_display_BS_staking_review();
int ui_display_swap_review();
int ui_display_review_with_warning();
int ui_display_confirm();
int ui_display_generic_config();
int ui_display_generic_review();
int ui_display_generic_settings();
int ui_display_keypad_digits();
int ui_display_keypad_pin();
int ui_display_navigation(uint8_t nav_type);
int ui_play_sound(uint8_t tune_type);
int ui_display_action(void);
