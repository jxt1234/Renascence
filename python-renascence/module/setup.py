from setuptools import setup, find_packages
from distutils.extension import Extension
setup(
        name = 'Renascence',
        version = '0.1',
        packages = find_packages(),
        py_modules =['Renascence', 'RenascenceBasic'],
        ext_modules=[Extension("_RenascenceBasic",
            ["../../swig/renascence_basic_wrap.cxx"], include_dirs=['../../include/'],
            extra_link_args=["../../libGP.a"],
            ),
            ],
        )
