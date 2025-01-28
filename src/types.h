#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t

#include "bip32.h"

#include "constants.h"

/**
 * Enumeration with expected INS of APDU commands.
 */
typedef enum {
    GET_VERSION = 0x03,   /// version of the application
    GET_APP_NAME = 0x04,  /// name of the application
    TEST_USE_CASE_REVIEW = 0x05,
    TEST_USE_CASE_REVIEW_BLIND_SIGNING = 0x06,
    TEST_USE_CASE_STREAMING_REVIEW = 0x07,
    TEST_USE_CASE_STREAMING_REVIEW_BLIND_SIGNING = 0x08,
    TEST_USE_CASE_ADDRESS_REVIEW = 0x09,
    TEST_SPINNER = 0x0A,
    TEST_USE_CASE_STATIC_REVIEW = 0x0B,
    TEST_USE_CASE_LIGHT_REVIEW = 0x0C,
    TEST_USE_CASE_CONFIRM = 0x0D,
    TEST_USE_CASE_GENERIC_CONFIG = 0x0E,
    TEST_USE_CASE_GENERIC_REVIEW = 0x0F,
    TEST_USE_CASE_GENERIC_SETTINGS = 0x10,
    TEST_USE_CASE_KEYPAD = 0x11,
    TEST_USE_CASE_NAVIGATION = 0x12,
} command_e;

/**
 * Constants with expected P1 of APDU commands.
 */
#define P1_ADDR_REVIEW_SHORT         0x00
#define P1_ADDR_REVIEW_LONG          0x01
#define P1_ADDR_REVIEW_TAGS          0x02
#define P1_KEYPAD_DIGITS             0x00
#define P1_KEYPAD_PIN                0x01
#define P1_NAV_CONTENT_CENTERED_INFO 0x00
#define P1_NAV_CONTENT_INFO_BUTTON   0x01
#define P1_NAV_CONTENT_SWITCHES      0x02
#define P1_NAV_CONTENT_CHOICES       0x03
#define P1_NAV_CONTENT_BARS          0x04
