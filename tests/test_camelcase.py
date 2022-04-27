import pytest

from yascc.camelcase import decamelize

from .django_rest_framework_camelcase import underscoreize


def test_decamelize(camelized_dict: dict, decamelized_dict: dict) -> None:
    got = decamelize(camelized_dict)
    assert got == decamelized_dict


@pytest.mark.parametrize("impl", [decamelize, underscoreize], ids=["c", "drf"])
def test_decamelize_benchmark(benchmark, camelized_list_of_dict, impl):
    benchmark(impl, camelized_list_of_dict)


def test_decamelize_nested_dict(camelized_nested_dict, decamelized_nested_dict) -> None:
    assert decamelize(camelized_nested_dict) == decamelized_nested_dict


def test_decamelize_list_of_dict(camelized_list_of_dict, decamelized_list_of_dict) -> None:
    assert decamelize(camelized_list_of_dict) == decamelized_list_of_dict
