# -*- coding: utf-8 -*-

from cffi import FFI
from pathlib import Path


class Trivium(object):
    """
    """

    MAX_KEY_LEN = 10
    MAX_IV_LEN = 10

    def __init__(self, h_file_path=None, lib_trivium_path=None):
        # Get Trivium header and library paths
        if h_file_path is None:
            self.h_file_path = Path().absolute().as_posix()+'/../c-trivium/inc/trivium.h'
        else:
            self.h_file_path = h_file_path
        if lib_trivium_path is None:
            self.lib_trivium_path = Path().absolute().as_posix() + '/../c-trivium/lib/libtrivium.so'
        else:
            self.lib_trivium_path = lib_trivium_path

        # Instantiate cffi base object
        self.ffi = FFI()

        # Load Trivium header file
        with open(self.h_file_path) as h_file:
            out = ''
            for line in h_file.readlines():
                if not line.startswith('#'):
                    out += line.replace('MAX_KEY_LEN', str(self.MAX_KEY_LEN)).replace('MAX_IV_LEN', str(self.MAX_IV_LEN))
            self.ffi.cdef(out)

        # Load the shared library into ffi
        self.lib = self.ffi.dlopen(self.lib_trivium_path)

        self.context = self.ffi.new('TRIVIUM_ctx*')

    def initialize(self, key, iv):
        """
        """
        # Get key and iv length
        keylen = len(key)
        ivlen = len(iv)
        # Initialialize Trivium
        self.lib.TRIVIUM_init(
            self.context,
            self.ffi.new(f'uint8_t[{keylen}]', key),
            self.ffi.new(f'uint8_t[{ivlen}]', iv),
            self.ffi.cast('uint8_t', keylen),
            self.ffi.cast('uint8_t', ivlen)
        )

    def update(self, n):
        """
        """
        output = self.ffi.new(f'uint32_t[{n}]')
        # Generate n*32 bits of key stream
        self.lib.TRIVIUM_genkeystream32(
            self.context,
            output,
            self.ffi.cast('uint32_t', n)
        )

        return output

    def finalize(self):
        """
        """
        pass
