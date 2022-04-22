"""
YET ANOTHER STRING CASE CONVERTER
"""
from importlib.metadata import PackageNotFoundError

try:
    import importlib.metadata as importlib_metadata
except ModuleNotFoundError:
    import importlib_metadata

try:
    __version__ = importlib_metadata.version(__name__)
except PackageNotFoundError:
    __version__ = "dev"
