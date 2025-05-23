# Trivium Cipher - C Implementation

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#build)
[![C Standard](https://img.shields.io/badge/C-C99-blue.svg)](#requirements)

A C implementation of the Trivium stream cipher, based on the official specification from [eCrypt](https://www.ecrypt.eu.org/stream/e2-trivium.html).

## About

Trivium is a synchronous stream cipher designed to generate up to 2^64 bits of key stream from an 80-bit secret key and an 80-bit initialization vector (IV). This implementation focuses on learning bitwise operations and Linear Feedback Shift Registers (LFSRs) in C.

## Features

- ✅ Compatible with official Trivium specification
- ✅ Shared library build support
- ✅ Memory-safe implementation
- ✅ Cross-platform compatibility
- ✅ Comprehensive security compilation flags

## Requirements

- **CMake** 3.25.1 or higher
- **GCC** or **Clang** compiler with C99 support
- **Make** build system

## Installation

### Quick Start

```bash
git clone https://github.com/rbrenaget/trivium-cipher.git
cd trivium-cipher/c-trivium
cmake .
make
```

### Building the Shared Library

```bash
# Navigate to the C implementation directory
cd c-trivium

# Generate build files
cmake .

# Compile the shared library
make

# Install library and headers (optional)
make install
```

The compiled library will be available in `lib/libtrivium.so`.

### Custom Installation Path

```bash
cmake . -DCMAKE_INSTALL_PREFIX=/your/custom/path
make install
```

## Usage

### Basic Example

```c
#include "trivium.h"

int main() {
    TRIVIUM_ctx ctx;
    uint8_t key[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    uint8_t iv[10] = {0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
    uint32_t output[10];
    
    // Initialize Trivium context
    TRIVIUM_init(&ctx, key, iv, 10, 10);
    
    // Generate keystream
    TRIVIUM_genkeystream32(&ctx, output, 10);
    
    return 0;
}
```

### API Reference

#### Core Functions

- `TRIVIUM_init(ctx, key, iv, keylen, ivlen)` - Initialize the cipher context
- `TRIVIUM_genkeystream32(ctx, output, n)` - Generate 32-bit keystream blocks
- `TRIVIUM_keysetup(ctx, key)` - Set up the secret key
- `TRIVIUM_ivsetup(ctx, iv)` - Set up the initialization vector

## Testing

### Running Tests

```bash
# Build the main library first
cd c-trivium
cmake .
make

# Build and run tests
make test
# Or use CTest
ctest
```

### Test Coverage

The current test suite verifies:
- ✅ Keystream generation correctness
- ✅ Basic functionality validation

> **Note**: Tests are not exhaustive and are primarily for verification purposes.

## Security Features

This implementation includes several security-focused compilation flags:

- `-fstack-protector-strong` - Stack smashing protection
- `-D_FORTIFY_SOURCE=2` - Buffer overflow detection
- `-Wl,-z,relro,-z,now` - Relocation hardening
- `-Wall -Wextra -Werror` - Comprehensive warnings

## Roadmap

- [ ] Add comprehensive unit tests
- [ ] Implement additional keystream formats (8-bit, 16-bit)
- [ ] Performance optimizations
- [ ] Add benchmarking suite
- [ ] Documentation improvements

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## References

- [eCrypt Trivium Specification](https://www.ecrypt.eu.org/stream/e2-trivium.html)
- [Trivium Algorithm Description](https://www.ecrypt.eu.org/stream/p3ciphers/trivium/trivium_p3.pdf)

## Acknowledgments

- eCrypt consortium for the original Trivium specification
- The cryptographic community for ongoing security research

---

**⚠️ Security Notice**: This implementation is for educational and research purposes. For production use, please ensure proper security auditing and validation.