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

benchmarks (against drf implementation of camelization):
```

-------------------------------------------------------------------------------------------------------- benchmark: 8 tests --------------------------------------------------------------------------------------------------------
Name (time in ns)                               Min                     Max                   Mean                StdDev                 Median                 IQR             Outliers  OPS (Kops/s)            Rounds  Iterations
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_decamelize_benchmark[c]               541.0002 (1.0)       40,291.0009 (12.36)       679.8152 (1.11)       275.5550 (3.56)        666.9979 (1.11)      42.0005 (6.72)      169;4291    1,470.9881 (0.90)     195122           1
test_camelize_benchmark[c]                 593.7500 (1.10)       3,260.4501 (1.0)         611.6401 (1.0)         77.3632 (1.0)         602.1000 (1.0)        6.2499 (1.0)      825;10733    1,634.9484 (1.0)       77167          20
test_camelize_list_benchmark[c]          1,707.9983 (3.16)      97,790.9986 (29.99)     1,930.7755 (3.16)       670.5052 (8.67)      1,915.9997 (3.18)     124.9973 (20.00)     258;1546      517.9266 (0.32)     149076           1
test_decamelize_list_benchmark[c]        1,708.0019 (3.16)      48,791.0002 (14.96)     1,928.9105 (3.15)       508.9319 (6.58)      1,915.9997 (3.18)     125.0010 (20.00)      108;739      518.4274 (0.32)      46066           1
test_decamelize_benchmark[drf]           2,665.9982 (4.93)      46,499.9976 (14.26)     2,870.3662 (4.69)       720.6006 (9.31)      2,832.9996 (4.71)      83.0005 (13.28)     291;3385      348.3876 (0.21)      57010           1
test_camelize_benchmark[drf]             5,665.9992 (10.47)     69,041.0016 (21.18)     5,972.8728 (9.77)     1,180.5335 (15.26)     5,875.0011 (9.76)      83.0041 (13.28)     300;4081      167.4236 (0.10)      44777           1
test_camelize_list_benchmark[drf]       14,624.9986 (27.03)    145,416.0010 (44.60)    15,234.6664 (24.91)    1,909.1079 (24.68)    14,999.9978 (24.91)    166.0046 (26.56)     402;4213       65.6398 (0.04)      26345           1
test_decamelize_list_benchmark[drf]     26,917.0014 (49.75)     67,334.0001 (20.65)    27,983.8805 (45.75)    2,437.4111 (31.51)    27,417.0015 (45.54)    500.0002 (80.00)      131;509       35.7349 (0.02)       3892           1
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Legend:
  Outliers: 1 Standard Deviation from Mean; 1.5 IQR (InterQuartile Range) from 1st Quartile and 3rd Quartile.
  OPS: Operations Per Second, computed as 1 / Mean
```
