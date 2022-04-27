from typing import Any
from typing import Callable
from typing import MutableMapping
from typing import MutableSequence
from typing import TypeVar
from typing import Union

T = TypeVar("T", bound=Union[MutableMapping, MutableSequence, Any])


def apply(f: Callable[[str], str], d: T) -> T:
    q = [d]
    while q:
        o = q.pop()
        if isinstance(o, dict):
            for k in [*o]:
                v = o[f(k) if isinstance(k, str) else k] = o.pop(k)
                if isinstance(v, (dict, list)):
                    q.append(v)
        if isinstance(o, list):
            q.extend(o)
    return d
