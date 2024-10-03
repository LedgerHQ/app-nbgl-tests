#pragma once

#ifdef SCREEN_SIZE_NANO
#define LARGE_ICON         C_app_nbgl_tests_16px
#define LARGE_WARNING_ICON C_icon_warning
#else
#if (APP_TYPE == APP_DEMO_TYPE)
#define LARGE_ICON C_app_demo_64px
#else
#define LARGE_ICON C_app_nbgl_tests_64px
#endif
#define LARGE_WARNING_ICON C_Warning_64px
#endif
