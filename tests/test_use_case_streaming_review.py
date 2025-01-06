import pytest

from ragger.backend.interface import BackendInterface
from ragger.error import ExceptionRAPDU
from ragger.firmware import Firmware
from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.nbgl_command_sender import NBGLCommandSender, Errors, SW_OK


def test_use_case_streaming_review_accepted(backend: BackendInterface,
                                            scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with client.test_use_case_streaming_review():
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_blind_signed_streaming_review_accepted(backend: BackendInterface,
                                                         firmware: Firmware,
                                                         navigator: Navigator,
                                                         scenario_navigator: NavigateWithScenario,
                                                         test_name: str,
                                                         default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = [NavInsID.RIGHT_CLICK] if firmware.is_nano else [NavInsID.USE_CASE_CHOICE_REJECT]
    with client.test_use_case_blind_signed_streaming_review():
        navigator.navigate_and_compare(default_screenshot_path,
                                test_name+"/BS_screen",
                                instructions,
                                screen_change_after_last_instruction=False)
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


# display the long value field with more button
def test_use_case_streaming_review_accepted_with_more(firmware: Firmware,
                                                      backend: BackendInterface,
                                                      navigator: Navigator,
                                                      test_name: str,
                                                      default_screenshot_path: str) -> None:
    if firmware.is_nano:
        pytest.skip("Nano does not support legacy useCase on NBGL")

    client = NBGLCommandSender(backend)

    # Navigate in the main menu
    instructions = []
    if firmware is Firmware.STAX:
        instructions += [
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavIns(NavInsID.TOUCH, (190, 424)),
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.CANCEL_FOOTER_TAP ,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.USE_CASE_REVIEW_CONFIRM
        ]
    elif firmware is Firmware.FLEX:
        instructions += [
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavIns(NavInsID.TOUCH, (217, 360)),
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.CANCEL_FOOTER_TAP ,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.USE_CASE_REVIEW_CONFIRM
        ]

    assert len(instructions) > 0
    with client.test_use_case_streaming_review():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_streaming_review_refused(backend: BackendInterface,
                                           scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_streaming_review():
            scenario_navigator.review_reject()

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
