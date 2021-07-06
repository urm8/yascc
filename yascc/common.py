from functools import partial
from typing import Callable, Union


def apply(function: Callable[[str], str], obj: Union[dict, list]) -> Union[dict, list]:
    if not isinstance(obj, dict):
        if isinstance(obj, list):
            return [*map(partial(apply, function), obj)]
        return obj
    return {function(key): apply(function, value) for key, value in obj.items()}
