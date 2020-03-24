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

        # 
        self.ffi = FFI()

        # 
        with open(self.h_file_path) as h_file:
            out = ''
            for line in h_file.readlines():
                if not line.startswith('#'):
                    out += line.replace('MAX_KEY_LEN', str(self.MAX_KEY_LEN)).replace('MAX_IV_LEN', str(self.MAX_IV_LEN))
            self.ffi.cdef(out)

        # Load the shared library into ffi
        self.lib = self.ffi.dlopen(self.lib_trivium_path)

    def initialize(self, key, iv):
        """
        """
        pass

    def update(self):
        """
        """
        pass

    def finalize(self):
        """
        """
        pass



if __name__ == '__main__':
    key = [0x0]
    iv = [0x0]
    engine = Trivium()