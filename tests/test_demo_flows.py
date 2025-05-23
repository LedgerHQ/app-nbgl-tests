import pytest

from ledgered.devices import Device, DeviceType

from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.navigator.navigation_scenario import NavigateWithScenario

def test_app_demo_flow_send_BTC(navigator: Navigator,
                                scenario_navigator: NavigateWithScenario,
                                test_name: str) -> None:
    device = scenario_navigator.backend.device
    screenshot_path=scenario_navigator.screenshot_path
    # Navigate in the main menu
    if device.is_nano:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        instructions = [
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (200, 130))
        ]

    navigator.navigate_and_compare(screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")


def test_app_demo_flow_swap_1inch(navigator: Navigator,
                                  scenario_navigator: NavigateWithScenario,
                                  test_name: str) -> None:
    device = scenario_navigator.backend.device
    screenshot_path=scenario_navigator.screenshot_path
    if device.is_nano:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        # Navigate in the main menu
        instructions = [
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (200, 230)),
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (350 if device.type == DeviceType.STAX else 420,
                                    310 if device.type == DeviceType.STAX else 320)),
            NavIns(NavInsID.TOUCH, (50, 50)),
            NavIns(NavInsID.TOUCH, (50, 50)),
        ]

    navigator.navigate_and_compare(screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")


def test_app_demo_flow_BS_stake(navigator: Navigator,
                                scenario_navigator: NavigateWithScenario,
                                test_name: str) -> None:
    device = scenario_navigator.backend.device
    screenshot_path=scenario_navigator.screenshot_path
    if device.is_nano:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        # Navigate in the main menu
        instructions = [
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (200, 320))
        ]

    navigator.navigate_and_compare(screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve_with_warning(test_name=test_name+"/part2")


def test_app_demo_flow_SOL_receive(navigator: Navigator,
                                   scenario_navigator: NavigateWithScenario,
                                   test_name: str) -> None:
    device = scenario_navigator.backend.device
    screenshot_path=scenario_navigator.screenshot_path
    if device.is_nano:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        # Navigate in the main menu
        instructions = [
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (200, 420))
        ]

    navigator.navigate_and_compare(screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.address_review_approve(test_name=test_name+"/part2")


def test_app_demo_flow_ETH_warning(device: Device,
                                   navigator: Navigator,
                                   test_name: str,
                                   default_screenshot_path: str) -> None:
    if device.is_nano:
        pytest.skip("Nano does not support this use case with warning screen")

    instructions = [
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (200, 520)),
            NavInsID.INFO_HEADER_TAP,
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.USE_CASE_REVIEW_REJECT,
            NavInsID.INFO_HEADER_TAP,
            NavIns(NavInsID.TOUCH, (200, 150)),
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, (200, 150)),
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.SWIPE_CENTER_TO_LEFT,
            NavInsID.SWIPE_CENTER_TO_LEFT,
    ]
    if device.type == DeviceType.FLEX:
        instructions += [NavInsID.SWIPE_CENTER_TO_LEFT]
    instructions += [
            NavInsID.INFO_HEADER_TAP,
            NavIns(NavInsID.TOUCH, (200, 150)),
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.LEFT_HEADER_TAP,
            NavInsID.USE_CASE_REVIEW_CONFIRM,
        ]
    navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
