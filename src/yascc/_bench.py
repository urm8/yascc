import inspect
import timeit
from functools import partial


def _bench(module, test_string="thisIsVeryEasyString", number=100_000):
    getter = partial(getattr, module)
    for attr in dir(module):
        if not attr.startswith("to_"):
            continue
        func = getter(attr)
        if not callable(func):
            continue
        setup = f"from {module.__name__} import {attr}"
        print("*" * 80)
        print(f"testing {attr} func:")
        print(setup)
        print(inspect.getsource(func))
        print(f'Sample output for "{test_string}" is {func(test_string)}')
        print(f"Runs: {number}")
        time = timeit.timeit(f'{attr}("{test_string}")', setup=setup, number=number)
        print(f"Time: {time}")
