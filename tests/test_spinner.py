import time

from ragger.backend.interface import BackendInterface
from ragger.navigator import Navigator, NavInsID, NavIns

from application_client.nbgl_command_sender import NBGLCommandSender

def test_spinner(backend: BackendInterface,
                 navigator: Navigator,
                 test_name: str,
                 default_screenshot_path: str) -> None:
    device = backend.device
    client = NBGLCommandSender(backend)

    instructions = []
    if not device.is_nano:
        instructions += [
            NavIns(NavInsID.TOUCH),
            NavIns(NavInsID.TOUCH),
            NavIns(NavInsID.TOUCH),
            NavIns(NavInsID.TOUCH)
        ]

    backend.pause_ticker()
    with client.test_spinner():
        # Let Speculos fully process the APDU and render the spinner
        # before any screenshot is taken, to avoid a non-deterministic
        # extra tick advancing the animation in wait_for_screen_change().
        time.sleep(0.5)
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions)
        backend.wait_for_home_screen()
    backend.resume_ticker()
