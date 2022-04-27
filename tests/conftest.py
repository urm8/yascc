from copy import deepcopy

import pytest


@pytest.fixture
def camelized_dict():
    return {"someObject": {"thatShould": "be", "deCamelized": [{"evenIf": "its nested inside list"}]}}


@pytest.fixture
def decamelized_dict():
    return {"some_object": {"that_should": "be", "de_camelized": [{"even_if": "its nested inside list"}]}}


@pytest.fixture
def decamelized_nested_dict():
    return deepcopy(
        {
            "id": 123,
            "username": "abc",
            "first_name": "def",
            "last_name": "ghi",
            "date_joined": "jkl",
            "last_login": "mno",
            "email": "p",
            "logged_in": True,
            "is_superuser": True,
            "is_staff": True,
            "permissions": ["q"],
            "metadata": {"filters": {"proposals": {}}, "eggs": [{"1": "rabbit"}, {"2": "snake"}]},
        }
    )


@pytest.fixture
def decamelized_list_of_dict(decamelized_nested_dict):
    return [decamelized_nested_dict]


@pytest.fixture
def camelized_nested_dict():
    return deepcopy(
        {
            "id": 123,
            "username": "abc",
            "firstName": "def",
            "lastName": "ghi",
            "dateJoined": "jkl",
            "lastLogin": "mno",
            "email": "p",
            "loggedIn": True,
            "isSuperuser": True,
            "isStaff": True,
            "permissions": ["q"],
            "metadata": {"filters": {"proposals": {}}, "eggs": [{"1": "rabbit"}, {"2": "snake"}]},
        }
    )


@pytest.fixture
def camelized_list_of_dict(camelized_nested_dict):
    return [camelized_nested_dict]
