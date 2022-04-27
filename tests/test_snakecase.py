import json

import pytest

from yascc import snakecase

from .django_rest_framework_camelcase import camelize


def test_camelize(camelized_dict, decamelized_dict) -> None:
    got = snakecase.camelize(decamelized_dict)
    assert got == camelized_dict


def test_camelize_nested_dict(decamelized_nested_dict, camelized_nested_dict) -> None:
    assert snakecase.camelize(decamelized_nested_dict) == camelized_nested_dict


@pytest.mark.parametrize("impl", [snakecase.camelize, camelize], ids=["c", "drf"])
def test_camelize_benchmark(benchmark, decamelized_list_of_dict, impl):
    benchmark(impl, decamelized_list_of_dict)


def test_decamelize_list_of_dict(decamelized_list_of_dict, camelized_list_of_dict) -> None:
    got = snakecase.camelize(decamelized_list_of_dict)
    assert (
        got == camelized_list_of_dict
    ), f"{json.dumps(decamelized_list_of_dict, indent=4)} != {json.dumps(camelized_list_of_dict, indent=4)}"
