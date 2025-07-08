from ledgered.devices import Device, DeviceType

from ragger.backend.interface import BackendInterface
from ragger.navigator import Navigator, NavInsID, NavIns

from application_client.nbgl_command_sender import NBGLCommandSender


# In this test we check the behavior of the device main menu
def test_app_mainmenu(device: Device,
                      navigator: Navigator,
                      test_name: str,
                      default_screenshot_path: str) -> None:
    # Navigate in the main menu
    instructions = []
    if device.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
        ]
    else:
        specific_device_instructions = {
            DeviceType.STAX: [(200, 113), (200, 261)],
            DeviceType.FLEX: [(200, 113), (200, 300)],
            DeviceType.APEX_P: [(243, 90), (243, 211)]
        }

        instructions += [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, specific_device_instructions[device.type][0]),
            NavIns(NavInsID.TOUCH, specific_device_instructions[device.type][1]),
            NavInsID.USE_CASE_CHOICE_CONFIRM,
            NavIns(NavInsID.TOUCH, specific_device_instructions[device.type][1]),
            NavInsID.USE_CASE_SETTINGS_NEXT,
            NavInsID.USE_CASE_SETTINGS_NEXT,
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]

    assert len(instructions) > 0
    navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                   screen_change_before_first_instruction=False)


def test_generic_settings(backend: BackendInterface,
                          navigator: Navigator,
                          test_name: str,
                          default_screenshot_path: str) -> None:
    device = backend.device
    client = NBGLCommandSender(backend)

    instructions = []
    if device.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    elif device.type == DeviceType.STAX:
        instructions += [
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]
    elif device.type == DeviceType.FLEX:
        instructions += [
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]
    elif device.type == DeviceType.APEX_P:
        instructions += [
            NavIns(NavInsID.TOUCH, (243, 90)),
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]

    with client.test_generic_settings():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()


def test_generic_config(backend: BackendInterface,
                        navigator: Navigator,
                        test_name: str,
                        default_screenshot_path: str) -> None:
    device = backend.device
    client = NBGLCommandSender(backend)

    instructions = []
    if device.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    elif device.type == DeviceType.STAX:
        instructions += [
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]
    elif device.type == DeviceType.FLEX:
        instructions += [
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]
    elif device.type == DeviceType.APEX_P:
        instructions += [
            NavIns(NavInsID.TOUCH, (243, 90)),
            NavInsID.USE_CASE_SETTINGS_MULTI_PAGE_EXIT
        ]

    with client.test_generic_config():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()
