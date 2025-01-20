from ragger.backend.interface import BackendInterface
from ragger.firmware import Firmware
from ragger.navigator import Navigator, NavInsID

from application_client.nbgl_command_sender import NBGLCommandSender

def test_confirm(backend: BackendInterface,
                 firmware: Firmware,
                 navigator: Navigator,
                 test_name: str,
                 default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = []
    if firmware.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        instructions += [
            NavInsID.USE_CASE_CHOICE_CONFIRM,
        ]

    with client.test_confirm():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()
