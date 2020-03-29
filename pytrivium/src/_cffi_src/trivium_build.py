# -*- coding: utf-8 -*-

import cffi
import pathlib


dirname = pathlib.Path(__file__).parent.absolute()
h_trivium_path = dirname / 'c-trivium/trivium.h'
c_trivium_path = dirname / 'c-trivium/trivium.c'

with open(h_trivium_path.as_posix()) as fh:
    definitions = fh.read()

with open(c_trivium_path.as_posix()) as fc:
    sources = fc.read()

ffibuilder = cffi.FFI()

ffibuilder.set_source(
    '_trivium',
    definitions+sources,
    # libraries=[],
    # library_dirs=[],
    # extra_link_args=[],
)

ffibuilder.cdef(definitions)
