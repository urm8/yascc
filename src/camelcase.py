from __future__ import annotations

from typing import Any

from _case import _to_snake_case

from .common import apply


def to_snake_case(src: str) -> str:
    return str(_to_snake_case(src))


def decamelize(d: dict[str, Any] | list[Any]) -> dict[str, Any] | list[Any]:
    return apply(to_snake_case, d)
