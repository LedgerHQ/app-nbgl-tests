from ledgered.devices import DeviceType

from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.nbgl_command_sender import NBGLCommandSender, Errors

def test_use_case_address_review_accepted(navigator: Navigator,
                                          scenario_navigator: NavigateWithScenario,
                                          test_name: str) -> None:
    backend = scenario_navigator.backend
    device = backend.device
    screenshot_path=scenario_navigator.screenshot_path
    client = NBGLCommandSender(backend)

    instructions = []
    if not device.is_nano:
        instructions = [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.TOUCH,
            (
                40 if device.type == DeviceType.APEX_P else 100,
                500 if device.type == DeviceType.STAX else 410 if device.type == DeviceType.FLEX else 310,
            )),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
            NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
        ]

    with client.test_use_case_address_review():
        if len(instructions) > 0:
            navigator.navigate_and_compare(screenshot_path, test_name, instructions)
        else:
            scenario_navigator.address_review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS


def test_use_case_long_address_review_accepted(navigator: Navigator,
                                               scenario_navigator: NavigateWithScenario,
                                               test_name: str) -> None:
    backend = scenario_navigator.backend
    device = backend.device
    screenshot_path=scenario_navigator.screenshot_path
    client = NBGLCommandSender(backend)

    instructions = []
    if not device.is_nano:
        instructions = [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.TOUCH,
            (
                40 if device.type == DeviceType.APEX_P else 100,
                500 if device.type == DeviceType.STAX else 410 if device.type == DeviceType.FLEX else 310,
            )),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
            NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
        ]

    with client.test_use_case_long_address_review():
        if len(instructions) > 0:
            navigator.navigate_and_compare(screenshot_path, test_name, instructions)
        else:
            scenario_navigator.address_review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS


def test_use_case_long_address_review_accepted_with_tags(navigator: Navigator,
                                                         scenario_navigator: NavigateWithScenario,
                                                         test_name: str) -> None:
    backend = scenario_navigator.backend
    device = backend.device
    screenshot_path=scenario_navigator.screenshot_path
    client = NBGLCommandSender(backend)

    instructions = []
    if not device.is_nano:
        instructions = [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.TOUCH,
            (
                150 if device.type == DeviceType.APEX_P else 200,
                370 if device.type == DeviceType.STAX else 370 if device.type == DeviceType.FLEX else 250,
            )),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR),
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM),
            NavIns(NavInsID.USE_CASE_STATUS_DISMISS)
        ]

    with client.test_use_case_long_address_review_with_tags():
        if len(instructions) > 0:
            navigator.navigate_and_compare(screenshot_path, test_name, instructions)
        else:
            scenario_navigator.address_review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS
