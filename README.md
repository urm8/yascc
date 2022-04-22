# YACC (Yet another case converter) [![PyPI version](https://badge.fury.io/py/yascc.svg)](https://badge.fury.io/py/yascc)

Just wanted to shave off extra seconds from standard regex based camelcase to snakecase conversion implementation

```
pip install yascc
```

example usage:

```
>>> from yascc import camelcase
>>> camelcase.to_snake_case('yetAnotherCaseConverter')
'yet_another_case_converter'
>>> from yascc import snakecase
>>> snakecase.to_camel_case('yet_another_case_converter')
'yetAnotherCaseConverter'
>>>
```
