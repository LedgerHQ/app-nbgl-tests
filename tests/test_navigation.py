import pytest

from ragger.backend.interface import BackendInterface
from ragger.navigator import Navigator, NavInsID, NavIns
from ragger.firmware.touch.positions import POSITIONS
from ledgered.devices import DeviceType

from application_client.nbgl_command_sender import NBGLCommandSender


@pytest.mark.parametrize("mode", ["info", "button", "switch", "choice", "bar"])
def test_navigation(backend: BackendInterface,
                    navigator: Navigator,
                    test_name: str,
                    default_screenshot_path: str,
                    mode: str) -> None:
    device = backend.device
    client = NBGLCommandSender(backend)

    if device.is_nano:
        instructions = {
            "info":   [NavInsID.RIGHT_CLICK, NavInsID.BOTH_CLICK],
            "button": [NavInsID.RIGHT_CLICK, NavInsID.BOTH_CLICK],
            "switch": [
                NavInsID.BOTH_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.BOTH_CLICK],
            "choice": [
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.BOTH_CLICK],
            "bar": [
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.RIGHT_CLICK,
                NavInsID.BOTH_CLICK]
        }
    elif device.type == DeviceType.APEX_P:
        instructions = {
            "info":   [],
            "button": [],
            "switch": [NavIns(NavInsID.TOUCH, (243, 90))],
            "choice": [NavIns(NavInsID.TOUCH, POSITIONS["ChoiceList"][device.type][2])],
            "bar":    []
        }
        instructions[mode] += [NavInsID.LEFT_HEADER_TAP]
    else:
        instructions = {
            "info":   [],
            "button": [],
            "switch": [NavIns(NavInsID.TOUCH, (200, 113))],
            "choice": [NavIns(NavInsID.TOUCH, POSITIONS["ChoiceList"][device.type][2])],
            "bar":    []
        }
        instructions[mode] += [NavInsID.LEFT_HEADER_TAP]

    test_name += f"_{mode}"
    p1 = {
        "info": 0x00,
        "button": 0x01,
        "switch": 0x02,
        "choice": 0x03,
        "bar": 0x04
    }

    with client.test_navigation(p1[mode]):
        navigator.navigate_and_compare(default_screenshot_path, test_name, instructions[mode])
        backend.wait_for_home_screen()
