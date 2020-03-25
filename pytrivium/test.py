# -*- coding: utf-8 -*-

from trivium import Trivium


if __name__ == '__main__':
    # Set 6, vector# 3:
    key = [0xfa, 0xa7, 0x54, 0x01, 0xae, 0x5b, 0x08, 0xb5, 0x62, 0x0f]
    iv = [0xc7, 0x60, 0xf9, 0x92, 0x2b, 0xc4, 0x5d, 0xf6, 0x8f, 0x28]

    engine = Trivium()

    engine.initialize(key, iv)
    engine.update(2)
    engine.update(6)
    output = engine.finalize()

    print([hex(o) for o in output])