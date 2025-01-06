from ragger.backend.interface import BackendInterface
from ragger.firmware import Firmware
from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.nbgl_command_sender import NBGLCommandSender, SW_OK

def test_use_case_address_review_accepted(backend: BackendInterface,
                                          firmware: Firmware,
                                          navigator: Navigator,
                                          scenario_navigator: NavigateWithScenario,
                                          test_name: str,
                                          default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = []
    if not firmware.is_nano:
        instructions = [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.TOUCH, (100, 500 if firmware is Firmware.STAX else 410)),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
            NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
        ]

    with client.test_use_case_address_review():
        if len(instructions) > 0:
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        else:
            scenario_navigator.address_review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_long_address_review_accepted(backend: BackendInterface,
                                               firmware: Firmware,
                                               navigator: Navigator,
                                               scenario_navigator: NavigateWithScenario,
                                               test_name: str,
                                               default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = []
    if not firmware.is_nano:
        instructions = [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.TOUCH, (100, 500 if firmware is Firmware.STAX else 410)),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
            NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
        ]

    with client.test_use_case_long_address_review():
        if len(instructions) > 0:
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        else:
            scenario_navigator.address_review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_long_address_review_accepted_with_tags(backend: BackendInterface,
                                                         firmware: Firmware,
                                                         navigator: Navigator,
                                                         scenario_navigator: NavigateWithScenario,
                                                         test_name: str,
                                                         default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = []
    if not firmware.is_nano:
        instructions = [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.TOUCH, (200, 370 if firmware is Firmware.STAX else 370)),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
            NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
        ]

    with client.test_use_case_long_address_review_with_tags():
        if len(instructions) > 0:
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        else:
            scenario_navigator.address_review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK
