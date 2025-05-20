import pytest

from ledgered.devices import DeviceType

from ragger.backend.interface import BackendInterface
from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.firmware.touch.positions import STAX_BUTTON_LOWER_RIGHT, FLEX_BUTTON_LOWER_RIGHT

from application_client.nbgl_command_sender import NBGLCommandSender


def get_digit5_position(device_type: DeviceType) -> tuple[int, int]:
    if device_type == DeviceType.STAX:
        screen_height = 672  # px
        screen_width = 400  # px
        header_height = 88  # px
    else:
        screen_height = 600  # px
        screen_width = 480  # px
        header_height = 96  # px

    usable_height = screen_height - header_height
    digit_x = screen_width // 2
    digit_y = header_height + usable_height // 2
    return digit_x, digit_y


def get_enter_position(device_type: DeviceType) -> tuple[int, int]:
    if device_type == DeviceType.STAX:
        return STAX_BUTTON_LOWER_RIGHT
    return FLEX_BUTTON_LOWER_RIGHT


@pytest.mark.parametrize("mode", ["digits", "pin"])
def test_keypad(backend: BackendInterface,
                navigator: Navigator,
                test_name: str,
                default_screenshot_path: str,
                mode: str) -> None:
    device = backend.device
    if device.is_nano:
        pytest.skip("Nano needs speculos API_LEVEL 23 for this test")

    client = NBGLCommandSender(backend)

    instructions = []
    if device.is_nano:
        instructions += [
            NavInsID.BOTH_CLICK * 5
        ]
    else:
        digit5_pos = get_digit5_position(device.type)
        instructions += [
            NavIns(NavInsID.TOUCH, digit5_pos),
            NavIns(NavInsID.TOUCH, digit5_pos),
            NavIns(NavInsID.TOUCH, digit5_pos),
            NavIns(NavInsID.TOUCH, digit5_pos),
            NavIns(NavInsID.TOUCH, get_enter_position(device.type)),
        ]

    test_name += f"_{mode}"
    p1 = 0x00 if mode == "pin" else 0x01
    with client.test_keypad(p1):
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()
