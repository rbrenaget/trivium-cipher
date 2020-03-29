# -*- coding: utf-8 -*-

from pytrivium import Trivium

import time
import contextlib 


@contextlib.contextmanager
def measure(name=''):
    try:
        begin = time.time()
        yield
    finally:
        end = time.time() - begin
        print(f'Measure {name} - wall time: {end}')


def test():
    N = 1000000
    # Set 6, vector# 3:
    key = [0xfa, 0xa7, 0x54, 0x01, 0xae, 0x5b, 0x08, 0xb5, 0x62, 0x0f]
    iv = [0xc7, 0x60, 0xf9, 0x92, 0x2b, 0xc4, 0x5d, 0xf6, 0x8f, 0x28]

    engine_api = Trivium()
    engine_api.initialize(key, iv)

    with measure('API'):
        for _ in range(N):
            engine_api.update(1)


if __name__ == '__main__':
    test()