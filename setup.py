# -*- coding: utf-8 -*-

import setuptools


with open('README.md', 'r') as fh:
    long_description = fh.read()


setuptools.setup(
    name='pytrivium',
    version='1.0.3',
    author='Romain Brenaget',
    description='Python bindings to a C implementation of Trivium',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/rbrenaget/trivium-sw',
    package_dir={'': 'pytrivium'},
    packages=setuptools.find_packages(where='pytrivium', exclude=['_cffi_build', '_cffi_src.*']),
    datafiles=[
        ('c-trivium', ['c-trivium/lib/*.so', 'c-trivium/inc/*.h'])
    ],
    include_package_data=True,
    classifiers=[
        'Natural Language :: English',
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: POSIX :: Linux',
        'Topic :: Security :: Cryptography'
    ],
    python_requires='>=3.6',
    install_requires=['cffi'],
    setup_requires=['cffi'],
    ext_package='pytrivium',
    cffi_modules=['pytrivium/_cffi_build/pytrivium_build.py:ffibuilder']
)

