# Trivium - Software Implementation

In this repository you will find a C implementation of Trivium. This implementation is quite similar of the official one that you can find on [eCrypt](https://www.ecrypt.eu.org/stream/e2-trivium.html). 

This project was a way for me to learn more about bitwise operations and LFSRs in C.

The next objective is to create Python bindings with the generated C library.

## C Implementation

### Implemented Features

* Initialization of Trivium.
* Key stream generation (32-bit by rotation). 

### Future Work

* Instantiate Trivium as a CSPRNG (Cryptographically Secure Pseudo-Random Number Generator).
* Allow plain text encryption by xoring the plain text with the key stream.
* Make a generic implementation to allow 8-bit, 16-bit, 32-bit and 64-bit output by rotation.

### Build Shared Library

```bash
# Go to C Trivium working directory
$ cd c-trivium/
# Create a build directory and go in it
$ mkdir build && cd build/
# Run cmake to generate the project compilation files
$ cmake ..
# Run make to compile the shared library
$ make
# Run make install to move the library in c-trivium/lib/
$ make install
```

### Build Tests

These tests are not exhaustive. For now, it is not intended to provide complete unit test. It is just a way to verify the correctness of the output keystream.

```bash
$ cd c-trivium/
$ mkdir build && cd build/
$ cmake ..
$ make
$ ./libtriviumtest
```