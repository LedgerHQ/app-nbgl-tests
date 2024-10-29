from ragger.firmware import Firmware
from ragger.navigator import NavInsID, NavIns

def test_app_demo_flow_send_BTC(firmware, navigator, scenario_navigator, test_name, default_screenshot_path):
    # Navigate in the main menu
    instructions = [
        NavIns(NavInsID.TOUCH, (200, 520 if firmware is Firmware.STAX else 440)),
        NavIns(NavInsID.TOUCH, (200, 130))
    ]

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")

def test_app_demo_flow_swap_1inch(firmware, navigator, scenario_navigator, test_name, default_screenshot_path):
    # Navigate in the main menu
    instructions = [
        NavIns(NavInsID.TOUCH, (200, 520 if firmware is Firmware.STAX else 440)),
        NavIns(NavInsID.TOUCH, (200, 230)),
        NavIns(NavInsID.TOUCH, (230, 500 if firmware is Firmware.STAX else 420)),
        NavIns(NavInsID.TOUCH, (350 if firmware is Firmware.STAX else 420, 310 if firmware is Firmware.STAX else 320)),
        NavIns(NavInsID.TOUCH, (50, 50)),
        NavIns(NavInsID.TOUCH, (50, 50)),
    ]

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")

def test_app_demo_flow_BS_stake(firmware, navigator, scenario_navigator, test_name, default_screenshot_path):
    # Navigate in the main menu
    instructions = [
        NavIns(NavInsID.TOUCH, (200, 520 if firmware is Firmware.STAX else 440)),
        NavIns(NavInsID.TOUCH, (200, 320)),
        NavInsID.USE_CASE_REVIEW_REJECT
    ]

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.review_approve(test_name=test_name+"/part2")

def test_app_demo_flow_SOL_receive(firmware, navigator, scenario_navigator, test_name, default_screenshot_path):
    # Navigate in the main menu
    instructions = [
        NavIns(NavInsID.TOUCH, (200, 520 if firmware is Firmware.STAX else 440)),
        NavIns(NavInsID.TOUCH, (200, 420))
    ]

    navigator.navigate_and_compare(default_screenshot_path, test_name+"/part1", instructions,
                                   screen_change_before_first_instruction=False,
                                   screen_change_after_last_instruction=False)
    scenario_navigator.address_review_approve(test_name=test_name+"/part2")
