from distutils.core import setup, Extension

setup(
        name='xlocale',
        ext_modules=[Extension('xlocale', ['xlocale.c'])],
        classifiers=[
            'Intended Audience :: Developers',
            'License :: DFSG approved',
            'License :: OSI Approved :: BSD License',
            'Operating System :: OS Independent',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.6',
            'Programming Language :: Python :: 2.7',
            'Topic :: Software Development :: Libraries :: Python Modules',
            ],
        )
