import pytest

from yascc import snakecase

from .django_rest_framework_camelcase import camelize


def test_camelize(camelized_dict, decamelized_dict) -> None:
    got = snakecase.camelize(decamelized_dict)
    assert got == camelized_dict


@pytest.mark.parametrize("impl", [snakecase.camelize, camelize], ids=["c", "drf"])
def test_camelize_benchmark(benchmark, decamelized_dict, impl):
    benchmark(impl, decamelized_dict)
