import sys
from setuptools import setup
from distutils.core import Extension


version = '1.3.3'


macros = []
if sys.platform.startswith('freebsd') or sys.platform == 'darwin':
    macros.append(('PLATFORM_BSD', '1'))
elif 'linux' in sys.platform:
    macros.append(('_GNU_SOURCE', ''))
setup(
        name='xlocale',
        version=version,
        long_description=open('README.rst').read() + '\n' +
            open('changes.rst').read(),
        ext_modules=[Extension('xlocale',
            define_macros=macros, sources=['xlocale.c'])],
        classifiers=[
            'Intended Audience :: Developers',
            'License :: DFSG approved',
            'License :: OSI Approved :: BSD License',
            'Operating System :: OS Independent',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.6',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.4',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 3.6',
            'Topic :: Software Development :: Libraries :: Python Modules',
            ],
        keywords='xlocale locale i18n l10n',
        author='Wichert Akkerman',
        author_email='wichert@wiggy.net',
        url='https://github.com/wichert/xlocale',
        license='BSD',
        )
