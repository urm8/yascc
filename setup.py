import sys

from setuptools import Extension
from setuptools import setup

args = []
if sys.platform == "win32":
    args.append("/Ox")
else:
    args.append("-O3")

ext_modules = [Extension("_case", sources=["src/yascc/case.c"], extra_compile_args=args)]


setup(ext_modules=ext_modules)
