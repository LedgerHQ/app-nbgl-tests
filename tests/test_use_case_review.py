import pytest

from application_client.nbgl_command_sender import NBGLCommandSender, Errors
from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID


def test_use_case_review_accepted(backend, scenario_navigator):
    client = NBGLCommandSender(backend)

    with client.test_use_case_review():
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000

def test_use_case_blind_signed_review_accepted(navigator, backend, scenario_navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    with client.test_use_case_blind_signed_review():
        navigator.navigate_and_compare(default_screenshot_path,
                        test_name+"/part1",
                        [NavInsID.USE_CASE_CHOICE_REJECT],
                        screen_change_after_last_instruction=False)
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == 0x9000

def test_use_case_blind_signed_review_display_warning(navigator, backend, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
            NavInsID.USE_CASE_CHOICE_REJECT,
            NavInsID.INFO_HEADER_TAP,
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.USE_CASE_REVIEW_REJECT,
            NavInsID.USE_CASE_CHOICE_CONFIRM
        ]
    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_blind_signed_review():
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                    screen_change_before_first_instruction=True)



def test_use_case_review_refused(backend, scenario_navigator):
    client = NBGLCommandSender(backend)

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_review():
            scenario_navigator.review_reject()

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
