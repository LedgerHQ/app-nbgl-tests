import pytest

from ledgered.devices import DeviceType

from ragger.backend.interface import BackendInterface
from ragger.error import ExceptionRAPDU
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
                                                         scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with client.test_use_case_blind_signed_streaming_review():
        scenario_navigator.review_approve_with_warning()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


# display the long value field with more button
def test_use_case_streaming_review_accepted_with_more(backend: BackendInterface,
                                                      navigator: Navigator,
                                                      test_name: str,
                                                      default_screenshot_path: str) -> None:
    device = backend.device
    if device.is_nano:
        pytest.skip("Nano does not support legacy useCase on NBGL")

    client = NBGLCommandSender(backend)

    # specific coordinates for each device
    specific_device_instructions = {
        DeviceType.STAX: (190, 424),
        DeviceType.FLEX: (217, 360),
        DeviceType.APEX_P: (150, 250)
    }

    instructions = [
        NavInsID.SWIPE_CENTER_TO_LEFT,
        NavInsID.SWIPE_CENTER_TO_LEFT,
        NavInsID.SWIPE_CENTER_TO_LEFT,
        NavInsID.SWIPE_CENTER_TO_LEFT,
        NavIns(NavInsID.TOUCH, specific_device_instructions[device.type]),
        NavInsID.SWIPE_CENTER_TO_LEFT,
        NavInsID.CANCEL_FOOTER_TAP,
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
