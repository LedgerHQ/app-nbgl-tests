import pytest

from ragger.backend.interface import BackendInterface
from ragger.firmware import Firmware
from ragger.error import ExceptionRAPDU
from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.nbgl_command_sender import NBGLCommandSender, Errors, SW_OK


def test_use_case_static_review_accepted(backend: BackendInterface,
                                         firmware: Firmware,
                                         scenario_navigator: NavigateWithScenario) -> None:
    if firmware.is_nano:
        pytest.skip("Nano does not support legacy useCase on NBGL")

    client = NBGLCommandSender(backend)

    with client.test_use_case_static_review():
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_static_review_refused(backend: BackendInterface,
                                        firmware: Firmware,
                                        navigator: Navigator,
                                        test_name: str,
                                        default_screenshot_path: str) -> None:
    if firmware.is_nano:
        pytest.skip("Nano does not support legacy useCase on NBGL")

    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.USE_CASE_CHOICE_REJECT)
    ]

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_static_review():
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0


def test_use_case_light_review_accepted(backend: BackendInterface,
                                        firmware: Firmware,
                                        navigator: Navigator,
                                        test_name: str,
                                        default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = []
    if firmware.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        instructions += [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.USE_CASE_CHOICE_CONFIRM)
        ]

    with client.test_use_case_light_review():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_light_review_refused(backend: BackendInterface,
                                       firmware: Firmware,
                                       navigator: Navigator,
                                       test_name: str,
                                       default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = []
    if firmware.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        instructions += [
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
            NavIns(NavInsID.USE_CASE_CHOICE_REJECT),
            NavIns(NavInsID.USE_CASE_CHOICE_CONFIRM)
        ]

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_light_review():
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
