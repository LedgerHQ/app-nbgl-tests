from ragger.firmware import Firmware
from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.navigator.navigation_scenario import NavigateWithScenario

def test_app_demo_flow_send_BTC(firmware: Firmware,
                                navigator: Navigator,
                                scenario_navigator: NavigateWithScenario,
                                test_name: str,
                                default_screenshot_path: str) -> None:
    # Navigate in the main menu
    if firmware.is_nano:
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

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")


def test_app_demo_flow_swap_1inch(firmware: Firmware,
                                  navigator: Navigator,
                                  scenario_navigator: NavigateWithScenario,
                                  test_name: str,
                                  default_screenshot_path: str) -> None:
    if firmware.is_nano:
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
            NavIns(NavInsID.TOUCH, (350 if firmware is Firmware.STAX else 420,
                                    310 if firmware is Firmware.STAX else 320)),
            NavIns(NavInsID.TOUCH, (50, 50)),
            NavIns(NavInsID.TOUCH, (50, 50)),
        ]

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")


def test_app_demo_flow_BS_stake(firmware: Firmware,
                                navigator: Navigator,
                                scenario_navigator: NavigateWithScenario,
                                test_name: str,
                                default_screenshot_path: str) -> None:
    if firmware.is_nano:
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
            NavIns(NavInsID.TOUCH, (200, 320)),
            NavInsID.USE_CASE_REVIEW_REJECT
        ]

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")


def test_app_demo_flow_SOL_receive(firmware: Firmware,
                                   navigator: Navigator,
                                   scenario_navigator: NavigateWithScenario,
                                   test_name: str,
                                   default_screenshot_path: str) -> None:
    if firmware.is_nano:
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

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.address_review_approve(test_name=test_name+"/part2")
