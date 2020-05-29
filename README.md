# Trivium - Software Implementations

In this repository you will find a collection of Trivium software implementations.

## C Implementation

Simple pure C implementation of Trivium.

### Implemented Features

* Initialization of Trivium.
* Key stream generation (32-bit by rotation). 

### Future Work

* Instantiates Trivium as a CSPRNG (Cryptographically Secure Pseudo-Random Number Generator).
* Allows plain text encryption by xoring the plain text with the key stream.
* Makes a generic implementation to allow 8-bit, 16-bit, 32-bit and 64-bit output by rotation.

## Python Implementation

Python bindings to the C implementation.

### Implemented features

* Creation of Python bindings to the C implementation using `cffi` (API level, out-of-line). 
* Creation of the Trivium class with methods: initialize, update and finalize.
* Adding `pytrivium` package to PyPi.

### Future work

* Adds a stream cipher feature.
* Adds a pseudo-random number generator feature.
* Bindings with the future generic Trivium.
