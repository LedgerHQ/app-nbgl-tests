from ragger.backend.interface import BackendInterface
from ragger.navigator import Navigator, NavInsID, NavIns

from application_client.nbgl_command_sender import NBGLCommandSender

def test_spinner(backend: BackendInterface,
                 navigator: Navigator,
                 test_name: str,
                 default_screenshot_path: str) -> None:
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.TOUCH),
        NavIns(NavInsID.TOUCH),
        NavIns(NavInsID.TOUCH),
        NavIns(NavInsID.TOUCH)
    ]
    with client.test_spinner():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()
