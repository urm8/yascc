from __future__ import annotations

from typing import Any, Union
from .common import apply

from _case import _to_snake_case


def to_snake_case(src: str) -> str:
    return str(_to_snake_case(src))


def decamelize(d: Union[dict[str, Any], list[Any]]) -> Union[dict[str, Any], list[Any]]:
    return apply(to_snake_case, d)


if __name__ == "__main__":
    import sys

    from _bench import _bench

    _bench(sys.modules[__name__])
