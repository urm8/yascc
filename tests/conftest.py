import pytest


@pytest.fixture
def camelized_dict():
    return {"someObject": {"thatShould": "be", "deCamelized": [{"evenIf": "its nested inside list"}]}}


@pytest.fixture
def decamelized_dict():
    return {"some_object": {"that_should": "be", "de_camelized": [{"even_if": "its nested inside list"}]}}
