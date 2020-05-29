# Trivium - Python Implementation

Here is a Python implementation of Trivium. It uses `cffi` to bind Python with the C implementation.

This package provides short examples to learn about Python bindings and distributions.

## Installation

I advise you to install it in a virtualenv.

```bash
$ pip install pytrivium
```

## Demo

```python
from pytrivimum import Trivium

# Set 6, vector# 3:
key = [0xfa, 0xa7, 0x54, 0x01, 0xae, 0x5b, 0x08, 0xb5, 0x62, 0x0f]
iv = [0xc7, 0x60, 0xf9, 0x92, 0x2b, 0xc4, 0x5d, 0xf6, 0x8f, 0x28]

engine = Trivium()
engine.initialize(key, iv)

engine.update(8)

output = engine.finalize()

print([hex(i) for i in output])
```

```bash
# Require Python >= 3.6
python demo.py
```