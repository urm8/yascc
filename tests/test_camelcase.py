import pytest

from yascc.camelcase import decamelize
from yascc.camelcase import to_snake_case

from .django_rest_framework_camelcase import underscoreize


def test_to_snake_case():
    assert to_snake_case("testString") == "test_string"


def test_decamelize_dict(camelized_dict: dict, decamelized_dict: dict) -> None:
    got = decamelize(camelized_dict)
    assert got == decamelized_dict


@pytest.mark.parametrize(
    "arg,expected",
    [
        (None, None),
        ([None], [None]),
        ([{None: None}], [{None: None}]),
        ([{"someValue": None}], [{"some_value": None}]),
    ],
)
def test_decamelize(arg, expected):
    assert decamelize(arg) == expected


@pytest.mark.parametrize("impl", [decamelize, underscoreize], ids=["c", "drf"])
def test_decamelize_list_benchmark(benchmark, camelized_list_of_dict, impl):
    benchmark(impl, camelized_list_of_dict)


@pytest.mark.parametrize("impl", [decamelize, underscoreize], ids=["c", "drf"])
def test_decamelize_benchmark(benchmark, camelized_dict, impl):
    benchmark(impl, camelized_dict)


def test_decamelize_nested_dict(camelized_nested_dict, decamelized_nested_dict) -> None:
    assert decamelize(camelized_nested_dict) == decamelized_nested_dict


def test_decamelize_list_of_dict(camelized_list_of_dict, decamelized_list_of_dict) -> None:
    assert decamelize(camelized_list_of_dict) == decamelized_list_of_dict
