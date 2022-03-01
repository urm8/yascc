from typing import Callable, Union, Any, MutableMapping, MutableSequence


def apply(function: Callable[[str], str], obj: Union[MutableMapping, MutableSequence, Any]) -> Union[
    MutableMapping, MutableSequence]:
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
