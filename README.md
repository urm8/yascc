# YACC (Yet another case converter)

Just wanted to shave off extra seconds from standard regex based camelcase to snakecase conversion implementation

```
pip install yacc
```

example usage:

```
>>> from yacc import camelcase
>>> camelcase.to_snake_case('yetAnotherCaseConverter')
'yet_another_case_converter'
>>> from yacc import snakecase
>>> snakecase.to_camel_case('yet_another_case_converter')
'yetAnotherCaseConverter'
>>> 
```