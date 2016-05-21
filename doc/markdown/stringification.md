## Stringification

```#DOCTEST_CONFIG_USE_IOSFWD ```

```iosfwd ```

MSVC 2015: 20k after preprocessor - 7k loc (without the new lines)
drags cstdio, cstring and many others

GCC 5 - 1k of code without new lines and stuff

clang and libc++ - 400 loc

```catch.hpp ```

MSVC 2015: 85k after preprocessor - 42k loc (without the new lines)


```doctest.h ```
MSVC 2015: 2k after preprocessor - 250 loc (without the new lines)

---------------

[Home](readme.md#reference)
