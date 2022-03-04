import logging
from distutils.command.build_ext import build_ext
from distutils.core import Extension
from distutils.errors import CCompilerError
from distutils.errors import DistutilsExecError
from distutils.errors import DistutilsPlatformError

log = logging.getLogger(__name__)

ext_modules = [
    Extension("_case", sources=["src/yascc/case.c"], extra_compile_args=["-Wno-xxxx"]),
]


class BuildFailed(Exception):
    pass


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


def build(setup_kwargs):
    """
    This function is mandatory in order to build the extensions.
    """
    setup_kwargs.update({"ext_modules": ext_modules, "cmdclass": {"build_ext": ExtBuilder}})
