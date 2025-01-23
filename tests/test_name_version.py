from ragger.backend.interface import BackendInterface

from application_client.nbgl_command_sender import NBGLCommandSender
from application_client.nbgl_response_unpacker import unpack_get_app_and_version_response


from utils import verify_version, verify_name

# Test a specific APDU asking BOLOS (and not the app) the name and version of the current app
def test_get_app_and_version(backend: BackendInterface) -> None:
    # Use the app interface instead of raw interface
    client = NBGLCommandSender(backend)
    # Send the special instruction to BOLOS
    response = client.get_app_and_version()
    # Use an helper to parse the response, assert the values
    app_name, version = unpack_get_app_and_version_response(response.data)

    verify_name(app_name)
    verify_version(version)
