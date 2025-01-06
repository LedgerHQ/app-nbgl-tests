import pytest

from ragger.backend.interface import BackendInterface
from ragger.error import ExceptionRAPDU
from ragger.navigator import Navigator, NavInsID
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.nbgl_command_sender import NBGLCommandSender, Errors, SW_OK


def test_use_case_review_accepted(backend: BackendInterface,
                                  scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with client.test_use_case_review():
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_blind_signed_review_accepted(backend: BackendInterface,
                                               navigator: Navigator,
                                               scenario_navigator: NavigateWithScenario,
                                               test_name: str,
                                               default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    with client.test_use_case_blind_signed_review():
        navigator.navigate_and_compare(default_screenshot_path,
                        test_name+"/BS_screen",
                        [NavInsID.USE_CASE_CHOICE_REJECT],
                        screen_change_after_last_instruction=False)
        scenario_navigator.review_approve()

    status = client.get_async_response().status

    # Assert that we have received an approval
    assert status == SW_OK


def test_use_case_blind_signed_review_display_warning(backend: BackendInterface,
                                                      navigator: Navigator,
                                                      test_name: str,
                                                      default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = [
            NavInsID.USE_CASE_CHOICE_REJECT,
            NavInsID.INFO_HEADER_TAP,
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.USE_CASE_REVIEW_REJECT,
            NavInsID.USE_CASE_CHOICE_CONFIRM
        ]
    with pytest.raises(ExceptionRAPDU):
        with client.test_use_case_blind_signed_review():
            navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)



def test_use_case_review_refused(backend: BackendInterface,
                                 scenario_navigator: NavigateWithScenario) -> None:
    client = NBGLCommandSender(backend)

    with pytest.raises(ExceptionRAPDU) as e:
        with client.test_use_case_review():
            scenario_navigator.review_reject()

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
