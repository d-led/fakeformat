fakeformat
==========

This small header is intended as a fast c++ prototyping tool in cases where something like [.NET](http://msdn.microsoft.com/en-us/library/system.string.format.aspx#Format2_Example) `String.Format` or [Boost.Locale](http://www.boost.org/doc/libs/1_53_0/libs/locale/doc/html/localized_text_formatting.html)'s `format` is required, one doesn't need performance, doesn't want to use any large library and doesn't need fancy format specifiers.

Note that, unlike in `String.Format` or [python](http://docs.python.org/2/library/string.html#format-examples)'s `format`, Boost.Locale formatting language starts the placeholders at `1`. Hence, the configurability of `fakeformat`

usage
=====

Usage is extermely simple in case some prerequisites are met:
- you use STL for which there are default implementations and configurations
- the input parameters are `ofstream`-streamable
- you use single-byte character set

Apart from this short intro, check out the [tests](tree/master/test).

license
=======

Copyright 2013, Dmitry Ledentsov
MIT License: http://www.opensource.org/licenses/mit-license.php
