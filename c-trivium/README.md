# Trivium - C Implementation

Here is a C implementation which is quite similar to the official one that you can find at [eCrypt](https://www.ecrypt.eu.org/stream/e2-trivium.html).

This implementation was a way for me to learn more about bitwise operations and LFSRs in C.

The next objective is to made the C implementation more generic.

## Build Shared Library

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

## Build Tests

These tests are not exhaustive. For now, it is not intended to provide complete unit test. It is just a way to verify the correctness of the output keystream.

```bash
$ cd c-trivium/
$ mkdir build && cd build/
$ cmake ..
$ make
$ ./libtriviumtest
```