from typing import Any
from typing import Callable
from typing import MutableMapping
from typing import MutableSequence
from typing import TypeVar
from typing import Union

T = TypeVar("T", bound=Union[MutableMapping, MutableSequence, Any])


def apply(function: Callable[[str], str], obj: T) -> T:
    to_visit = [obj]
    while to_visit:
        candidate = to_visit.pop()
        if isinstance(candidate, MutableMapping):
            for key in [*candidate]:
                value = candidate[function(key)] = candidate[key]
                to_visit.append(value)
                del candidate[key]
        elif isinstance(candidate, MutableSequence):
            to_visit.extend(candidate)
    return obj
