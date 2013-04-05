fakeformat
==========

This small header is intended as a fast c++ prototyping tool in cases where something like .NET `String.Format` or Boost.Locale's `format` is required, one doesn't need performance, doesn't want to use any large library and doesn't need fancy format specifiers.

Note that, unlike in `String.Format` or [python](http://docs.python.org/2/library/string.html#format-examples)'s `format`, Boost.Locale formatting language starts the placeholders at `1`. Hence, the configurability of `fakeformat`

usage
=====

Apart from this short intro, check out the [tests](tree/master/test).

license
=======

Copyright 2013, Dmitry Ledentsov
MIT License: http://www.opensource.org/licenses/mit-license.php
