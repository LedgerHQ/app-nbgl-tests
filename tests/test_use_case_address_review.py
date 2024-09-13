from application_client.nbgl_command_sender import NBGLCommandSender
from ragger.firmware import Firmware
from ragger.navigator import NavInsID, NavIns

def test_use_case_address_review_accepted(backend, firmware, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.TOUCH, (100, 500 if firmware is Firmware.STAX else 410)),
        NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
        NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
        NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
    ]

    with client.test_use_case_address_review():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000

def test_use_case_long_address_review_accepted(backend, firmware, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.TOUCH, (100, 500 if firmware is Firmware.STAX else 410)),
        NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
        NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
        NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
    ]

    with client.test_use_case_long_address_review():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000

def test_use_case_long_address_review_accepted_with_tags(backend, firmware, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.TOUCH, (200, 370 if firmware is Firmware.STAX else 370)),
        NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
        NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
    ]

    with client.test_use_case_long_address_review_with_tags():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000
