from application_client.nbgl_command_sender import NBGLCommandSender
from ragger.navigator import NavInsID, NavIns

def test_spinner(backend, navigator, test_name, default_screenshot_path):
    client = NBGLCommandSender(backend)

    instructions = [
        NavIns(NavInsID.TOUCH),
        NavIns(NavInsID.TOUCH),
        NavIns(NavInsID.TOUCH),
        NavIns(NavInsID.TOUCH)
    ]
    with client.test_spinner():
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions,
                                screen_change_before_first_instruction=True)
        backend.wait_for_home_screen()

