from typing import Any
from typing import Callable
from typing import List
from typing import MutableMapping
from typing import MutableSequence
from typing import TypeVar
from typing import Union

T = TypeVar("T", bound=Union[MutableMapping, MutableSequence, Any])


def apply(function: Callable[[str], str], obj: T) -> T:
    _is_mapping = _is_mutable_mapping
    to_visit: List[T] = [obj]  # type: ignore
    while to_visit:
        c = to_visit.pop()
        for k in [*c]:
            c[function(k)] = v = c.pop(k)
            if isinstance(v, MutableSequence):
                to_visit.extend(filter(_is_mapping, v))
            elif isinstance(v, MutableMapping):
                to_visit.append(v)
    return obj


def _is_mutable_mapping(obj: MutableMapping):
    return isinstance(obj, MutableMapping)
