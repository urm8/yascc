from __future__ import annotations

from typing import Any

from _case import _to_camel_case


def to_camel_case(snake_case_string: str) -> str:
    return _to_camel_case(snake_case_string)


def camelize(d: dict[str, Any]) -> dict[str, Any]:
    return {_to_camel_case(key): value for key, value in d.items()}


if __name__ == "__main__":
    import sys

    from _bench import _bench

    _bench(sys.modules[__name__], test_string="this_is_very_easy_string")
