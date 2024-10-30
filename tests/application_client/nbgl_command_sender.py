from enum import IntEnum
from typing import Generator, List, Optional
from contextlib import contextmanager

from ragger.backend.interface import BackendInterface, RAPDU

MAX_APDU_LEN: int = 255

CLA: int = 0xE0

class P1(IntEnum):
    # Parameter 1 for first APDU number.
    P1_START = 0x00
    # Parameter 1 for maximum APDU number.
    P1_MAX   = 0x03
    # Parameter 1 for screen confirmation for GET_PUBLIC_KEY.
    P1_CONFIRM = 0x01

class P2(IntEnum):
    # Parameter 2 for last APDU to receive.
    P2_LAST = 0x00
    # Parameter 2 for more APDU to receive.
    P2_MORE = 0x80

class InsType(IntEnum):
    GET_VERSION    = 0x03
    GET_APP_NAME   = 0x04
    TEST_USE_CASE_REVIEW = 0x05
    TEST_USE_CASE_REVIEW_BLIND_SIGNING = 0x06
    TEST_USE_CASE_STREAMING_REVIEW = 0x07
    TEST_USE_CASE_STREAMING_REVIEW_BLIND_SIGNING = 0x08
    TEST_USE_CASE_ADDRESS_REVIEW = 0x09
    TEST_SPINNER = 0x0A
    TEST_USE_CASE_STATIC_REVIEW = 0x0B
    TEST_USE_CASE_LIGHT_REVIEW = 0x0C

class Errors(IntEnum):
    SW_DENY                    = 0x6985
    SW_WRONG_P1P2              = 0x6A86
    SW_WRONG_DATA_LENGTH       = 0x6A87
    SW_INS_NOT_SUPPORTED       = 0x6D00
    SW_CLA_NOT_SUPPORTED       = 0x6E00
    SW_WRONG_RESPONSE_LENGTH   = 0xB000
    SW_DISPLAY_BIP32_PATH_FAIL = 0xB001
    SW_DISPLAY_ADDRESS_FAIL    = 0xB002
    SW_DISPLAY_AMOUNT_FAIL     = 0xB003
    SW_WRONG_TX_LENGTH         = 0xB004
    SW_TX_PARSING_FAIL         = 0xB005
    SW_TX_HASH_FAIL            = 0xB006
    SW_BAD_STATE               = 0xB007
    SW_SIGNATURE_FAIL          = 0xB008

SW_OK: int = 0x9000

def split_message(message: bytes, max_size: int) -> List[bytes]:
    return [message[x:x + max_size] for x in range(0, len(message), max_size)]


class NBGLCommandSender:
    def __init__(self, backend: BackendInterface) -> None:
        self.backend = backend


    def get_app_and_version(self) -> RAPDU:
        return self.backend.exchange(cla=0xB0,  # specific CLA for BOLOS
                                     ins=0x01,  # specific INS for get_app_and_version
                                     p1=P1.P1_START,
                                     p2=P2.P2_LAST,
                                     data=b"")


    def get_version(self) -> RAPDU:
        return self.backend.exchange(cla=CLA,
                                     ins=InsType.GET_VERSION,
                                     p1=P1.P1_START,
                                     p2=P2.P2_LAST,
                                     data=b"")


    def get_app_name(self) -> RAPDU:
        return self.backend.exchange(cla=CLA,
                                     ins=InsType.GET_APP_NAME,
                                     p1=P1.P1_START,
                                     p2=P2.P2_LAST,
                                     data=b"")

    @contextmanager
    def test_spinner(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_SPINNER,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_REVIEW,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_blind_signed_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_REVIEW_BLIND_SIGNING,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_streaming_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_STREAMING_REVIEW,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_blind_signed_streaming_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_STREAMING_REVIEW_BLIND_SIGNING,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_address_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_ADDRESS_REVIEW,
                                         p1=0x00,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_long_address_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_ADDRESS_REVIEW,
                                         p1=0x01,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_long_address_review_with_tags(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_ADDRESS_REVIEW,
                                         p1=0x02,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_static_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_STATIC_REVIEW,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    @contextmanager
    def test_use_case_light_review(self) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.TEST_USE_CASE_LIGHT_REVIEW,
                                         p1=P1.P1_START,
                                         p2=P2.P2_LAST,
                                         data=b"") as response:
            yield response

    def get_async_response(self) -> Optional[RAPDU]:
        return self.backend.last_async_response