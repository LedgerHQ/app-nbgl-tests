import pytest
from application_client.nbgl_command_sender import CLA, Errors, InsType, NBGLCommandSender
from ledgered.devices import DeviceType
from ragger.backend.interface import BackendInterface
from ragger.error import ExceptionRAPDU
from ragger.navigator import Navigator, NavIns, NavInsID
from ragger.navigator.navigation_scenario import NavigateWithScenario


def test_use_case_streaming_review_accepted(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with client.test_use_case_streaming_review():
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS


def test_use_case_blind_signed_streaming_review_accepted(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = NBGLCommandSender(backend)

    with client.test_use_case_blind_signed_streaming_review():
        scenario_navigator.review_approve_with_warning()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS


# display the long value field with more button
def test_use_case_streaming_review_accepted_with_more(
    backend: BackendInterface,
    navigator: Navigator,
    test_name: str,
    default_screenshot_path: str,
) -> None:
    device = backend.device
    if device.is_nano:
        pytest.skip("Nano does not support legacy useCase on NBGL")

    client = NBGLCommandSender(backend)

    # specific coordinates for each device
    specific_device_instructions = {
        DeviceType.STAX: (190, 424),
        DeviceType.FLEX: (217, 360),
        DeviceType.APEX_P: (150, 250),
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
        NavInsID.USE_CASE_REVIEW_CONFIRM,
    ]

    assert len(instructions) > 0
    with client.test_use_case_streaming_review():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS


def test_use_case_streaming_review_refused(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_streaming_review():
            scenario_navigator.review_reject()

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0


# Navigate an advanced streaming review up to its last page, open and close the
# security report from the top-right button, then approve.
def _advanced_streaming_review_accepted(
    backend: BackendInterface,
    navigator: Navigator,
    test_name: str,
    default_screenshot_path: str,
    threat: bool,
) -> None:
    if backend.device.is_nano:
        pytest.skip("Web3 Checks security report is only reachable on touch devices")

    client = NBGLCommandSender(backend)

    snap_idx = 0
    with client.test_use_case_advanced_streaming_review(threat):
        if threat:
            # Accept the risk on the initial warning page
            navigator.navigate_and_compare(default_screenshot_path, test_name, [NavInsID.USE_CASE_CHOICE_REJECT])
            snap_idx = 1
        navigator.navigate_until_text_and_compare(
            NavInsID.SWIPE_CENTER_TO_LEFT,
            [NavInsID.RIGHT_HEADER_TAP, NavInsID.LEFT_HEADER_TAP, NavInsID.USE_CASE_REVIEW_CONFIRM],
            "Sign transaction",
            default_screenshot_path,
            test_name,
            screen_change_before_first_instruction=not threat,
            snap_start_idx=snap_idx,
        )

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == Errors.SW_SUCCESS


def test_use_case_advanced_streaming_review_no_threat_accepted(
    backend: BackendInterface,
    navigator: Navigator,
    test_name: str,
    default_screenshot_path: str,
) -> None:
    _advanced_streaming_review_accepted(backend, navigator, test_name, default_screenshot_path, False)


def test_use_case_advanced_streaming_review_threat_accepted(
    backend: BackendInterface,
    navigator: Navigator,
    test_name: str,
    default_screenshot_path: str,
) -> None:
    _advanced_streaming_review_accepted(backend, navigator, test_name, default_screenshot_path, True)


def test_use_case_advanced_streaming_review_wrong_p1p2(backend: BackendInterface) -> None:
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=InsType.TEST_USE_CASE_ADVANCED_STREAMING_REVIEW, p1=2)
    assert e.value.status == Errors.SW_WRONG_P1P2
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=InsType.TEST_USE_CASE_ADVANCED_STREAMING_REVIEW, p2=1)
    assert e.value.status == Errors.SW_WRONG_P1P2
