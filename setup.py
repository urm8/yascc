import logging
import sys
from distutils.command.build_ext import build_ext
from distutils.core import Extension
from distutils.errors import CCompilerError
from distutils.errors import DistutilsExecError
from distutils.errors import DistutilsPlatformError

from setuptools import Extension
from setuptools import setup

log = logging.getLogger(__name__)

args = []
if sys.platform == "win32":
    args.append("/Ox")
else:
    args.append("-O3")

ext_modules = [
    Extension(
        "_case", sources=["src/yascc/case.c", "src/yascc/stack.c"], include_dirs=["src/yascc"], extra_compile_args=args
    )
]


class ExtBuilder(build_ext):
    def run(self):
        try:
            build_ext.run(self)
        except (DistutilsPlatformError, FileNotFoundError) as e:
            log.error("Could not compile C extension.")
            raise e from None

    def build_extension(self, ext):
        try:
            build_ext.build_extension(self, ext)
        except (
            CCompilerError,
            DistutilsExecError,
            DistutilsPlatformError,
            ValueError,
        ) as e:
            log.error("Could not compile C extension.")
            raise e from None


setup(ext_modules=ext_modules, cmdclass={"build_ext": ExtBuilder})
