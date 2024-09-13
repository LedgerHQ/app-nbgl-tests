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
    TEST_USE_CASE_LIGHT_REVIEW = 0x0C
} command_e;
