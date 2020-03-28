# -*- coding: utf-8 -*-

from cffi import FFI
from pathlib import Path


class TriviumABI(object):
    """Trivium class with bindings to the C library using cffi (ABI Mode).
    :param h_file_path: Path to the Trivium hearder file (default is None).
    :type h_file_path: str
    :param lib_trivium_path: Path to the Trivium library (default is None).
    :type lib_trivium_path: str
    """

    MAX_KEY_LEN = 10
    MAX_IV_LEN = 10

    def __init__(self, h_file_path=None, lib_trivium_path=None):
        # Gets Trivium header and library paths
        dirname = Path(__file__).parent.absolute().as_posix()
        if h_file_path is None:
            self.h_file_path = dirname + '/../../c-trivium/inc/trivium.h'
        else:
            self.h_file_path = h_file_path
        if lib_trivium_path is None:
            self.lib_trivium_path = dirname + '/../../c-trivium/lib/libtrivium.so'
        else:
            self.lib_trivium_path = lib_trivium_path

        # Instantiate cffi base object
        self.ffi = FFI()

        # Loads Trivium header file
        with open(self.h_file_path) as h_file:
            out = ''
            for line in h_file.readlines():
                if not line.startswith('#'):
                    out += line.replace('MAX_KEY_LEN', str(self.MAX_KEY_LEN)).replace('MAX_IV_LEN', str(self.MAX_IV_LEN))
            self.ffi.cdef(out)

        # Loads the shared library into ffi
        self.lib = self.ffi.dlopen(self.lib_trivium_path)

        self.context = None
        self.keystream = []

    def initialize(self, key, iv):
        """Initializes the Trivium context.
        :param key: The secret key (10 bytes max)
        :type key: list[uint8_t,]
        :param iv: The initialization vector (10 bytes max)
        :type iv: list[uint8_t,]
        """
        # Gets key and iv lengths
        keylen = len(key)
        ivlen = len(iv)

        # Verifies key and iv lengths
        if keylen > self.MAX_KEY_LEN:
            raise Exception(f'Maximum key length is {self.MAX_KEY_LEN}')
        if ivlen > self.MAX_IV_LEN:
            raise Exception(f'Maximum iv length is {self.MAX_IV_LEN}')

        # Initialializes Trivium
        self.context = self.ffi.new('TRIVIUM_ctx*')
        self.lib.TRIVIUM_init(
            self.context,
            self.ffi.new(f'uint8_t[{keylen}]', key),
            self.ffi.new(f'uint8_t[{ivlen}]', iv),
            self.ffi.cast('uint8_t', keylen),
            self.ffi.cast('uint8_t', ivlen)
        )

    def update(self, n):
        """Processes n*32-bit key stream generation.
        :param n: The number of 32-bit key stream to generate.
        :type n: int
        """
        output = self.ffi.new(f'uint32_t[{n}]')
        # Generates n*32 bits of key stream
        self.lib.TRIVIUM_genkeystream32(
            self.context,
            output,
            self.ffi.cast('uint32_t', n)
        )
        # Store generated values
        self.keystream.extend([o for o in output])

    def finalize(self):
        """Finalizes the Tricium constext and returns the generated key stream.
        :returns: The key stream generated.
        :rtype: list
        """
        self.context = None

        return self.keystream
