# -*- coding: utf-8 -*-

from pytrivium.bindings._trivium import ffi, lib


class Trivium(object):
    """Trivium class with bindings to the C library using cffi.
    """

    MAX_KEY_LEN = 10
    MAX_IV_LEN = 10

    def __init__(self):
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
        self.context = ffi.new('TRIVIUM_ctx*')
        lib.TRIVIUM_init(
            self.context,
            ffi.new(f'uint8_t[{keylen}]', key),
            ffi.new(f'uint8_t[{ivlen}]', iv),
            ffi.cast('uint8_t', keylen),
            ffi.cast('uint8_t', ivlen)
        )

    def update(self, n):
        """Processes n*32-bit key stream generation.
        :param n: The number of 32-bit key stream to generate.
        :type n: int
        """
        output = ffi.new(f'uint32_t[{n}]')
        # Generates n*32 bits of key stream
        lib.TRIVIUM_genkeystream32(
            self.context,
            output,
            ffi.cast('uint32_t', n)
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