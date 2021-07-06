from typing import Callable, Union


def apply(function: Callable[[str], str], obj: Union[dict, list]) -> Union[dict, list]:
    if not isinstance(obj, dict):
        if isinstance(obj, list):
            return [*map(function, obj)]
        return obj
    return {function(key): value for key, value in obj.items()}
