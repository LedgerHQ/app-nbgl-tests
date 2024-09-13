import pytest

from application_client.nbgl_command_sender import NBGLCommandSender, Errors
from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID, NavIns


def test_use_case_static_review_accepted(backend, scenario_navigator):
    client = NBGLCommandSender(backend)

    with client.test_use_case_static_review():
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000

def test_use_case_static_review_refused(backend, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.USE_CASE_CHOICE_REJECT)
    ]

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_static_review():
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0

def test_use_case_light_review_accepted(backend, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)
    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.USE_CASE_CHOICE_CONFIRM)
    ]

    with client.test_use_case_light_review():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000

def test_use_case_light_review_refused(backend, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.SWIPE_CENTER_TO_LEFT),
        NavIns(NavInsID.USE_CASE_CHOICE_REJECT),
        NavIns(NavInsID.USE_CASE_CHOICE_CONFIRM)
    ]

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_light_review():
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
