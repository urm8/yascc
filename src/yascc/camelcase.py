from __future__ import annotations

from typing import Any

from _case import _decamelize
from _case import _to_snake_case


def to_snake_case(src: str) -> str:
    return _to_snake_case(src)


def decamelize(d: dict[str, Any] | list[Any]) -> dict[str, Any] | list[Any]:
    return _decamelize(d)
