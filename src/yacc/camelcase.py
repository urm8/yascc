from __future__ import annotations

from typing import Any

from _case import _to_snake_case


def to_snake_case(src: str) -> str:
    return _to_snake_case(src)


def decamelize(d: dict[str, Any]) -> dict[str, Any]:
    return {_to_snake_case(key): value for key, value in d.items()}


if __name__ == "__main__":
    import sys

    from _bench import _bench

    _bench(sys.modules[__name__])
