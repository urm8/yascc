from __future__ import annotations

from typing import Any

from _case import _camelize
from _case import _to_camel_case


def to_camel_case(snake_case_string: str) -> str:
    """Converts some_thing to someThing."""
    return str(_to_camel_case(snake_case_string))


def camelize(d: dict[str, Any]) -> dict[str, Any]:
    """Converts {'some_thing': {'having_value': 'like_so'}} to {'someThing': {'havingValue': 'like_so'}}."""
    return _camelize(d)
