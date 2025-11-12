from ragger.backend.interface import BackendInterface
from ragger.navigator import Navigator, NavInsID

from application_client.nbgl_command_sender import NBGLCommandSender

def test_confirm(backend: BackendInterface,
                 navigator: Navigator,
                 test_name: str,
                 default_screenshot_path: str) -> None:
    device = backend.device
    client = NBGLCommandSender(backend)

    instructions = []
    if device.is_nano:
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

def test_action(backend: BackendInterface,
                navigator: Navigator,
                test_name: str,
                default_screenshot_path: str) -> None:
    device = backend.device
    client = NBGLCommandSender(backend)

    instructions = []
    if device.is_nano:
        instructions += [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
        ]
    else:
        instructions += [
            NavInsID.CENTERED_FOOTER_TAP,
        ]

    with client.test_action():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()
