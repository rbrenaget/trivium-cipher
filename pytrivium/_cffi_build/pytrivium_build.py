# -*- coding: utf-8 -*-

import cffi
import pathlib


dirname = pathlib.Path(__file__).parent.absolute()
h_trivium_path = dirname / '../../c-trivium/inc/trivium.h'
lib_trivium_path = dirname / '../../c-trivium/lib/'

ffibuilder = cffi.FFI()

ffibuilder.set_source('_trivium',
    f"""
        #include "{h_trivium_path.as_posix()}"
    """,
    libraries=['trivium'],
    library_dirs=[lib_trivium_path.as_posix()],
    extra_link_args=[f'-Wl,-rpath,{lib_trivium_path.as_posix()}'],
)

ffibuilder.cdef("""
    typedef struct {
        uint8_t keylen;
        uint8_t ivlen;

        uint32_t lfsr_a[3];
        uint32_t lfsr_b[3];
        uint32_t lfsr_c[4];

        uint8_t key[10];
        uint8_t iv[10];
    } TRIVIUM_ctx;

    void TRIVIUM_init(TRIVIUM_ctx* ctx, const uint8_t key[], const uint8_t iv[], uint8_t keylen, uint8_t ivlen);
    void TRIVIUM_keysetup(TRIVIUM_ctx* ctx, const uint8_t key[]);
    void TRIVIUM_ivsetup(TRIVIUM_ctx* ctx, const uint8_t iv[]);
    void TRIVIUM_genkeystream32(TRIVIUM_ctx* ctx, uint32_t output[], uint32_t n);
""")


# if __name__ == "__main__":
#     ffibuilder.compile(verbose=True)