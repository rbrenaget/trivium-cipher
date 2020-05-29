# -*- coding: utf-8 -*-

import setuptools
import pathlib
import shutil


with open('README.md', 'r') as fh:
    long_description = fh.read()


setuptools.setup(
    name='pytrivium',
    version='1.0.7',
    author='Romain Brenaget',

    description='Python bindings to a C implementation of Trivium',
    long_description=long_description,
    long_description_content_type='text/markdown',
    license='MIT',
    url='https://github.com/rbrenaget/trivium-sw/pytrivium',

    classifiers=[
        'Natural Language :: English',
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: POSIX :: Linux',
        'Topic :: Security :: Cryptography'
    ],

    package_dir={'': 'src'},
    packages=setuptools.find_packages(where='src', exclude=['_cffi_src', '_cffi_src.*']),
    include_package_data=True,

    python_requires='>=3.6',

    install_requires=['cffi'],
    setup_requires=['cffi'],
    zip_safe=False,
    ext_package='pytrivium.bindings',
    cffi_modules=['src/_cffi_src/trivium_build.py:ffibuilder'],
)
