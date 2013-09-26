fakeformat
==========

[![Build Status](https://travis-ci.org/d-led/fakeformat.png?branch=master)](https://travis-ci.org/d-led/fakeformat)

This small library is intended as a fast c++ prototyping tool in cases where something like [.NET](http://msdn.microsoft.com/en-us/library/system.string.format.aspx#Format2_Example) `String.Format` or [Boost.Locale](http://www.boost.org/doc/libs/1_53_0/libs/locale/doc/html/localized_text_formatting.html)'s `format` is required, one doesn't need performance, doesn't want to use any large library and doesn't need fancy format specifiers.

Note that, unlike in `String.Format` or [python](http://docs.python.org/2/library/string.html#format-examples)'s `format`, Boost.Locale formatting language starts the placeholders at `1`. Hence, the configurability of `fakeformat`.

usage
=====

building
--------

Include `fakeformat.hpp` and compile `fakeformat.cpp`

The [premake4](http://industriousone.com/premake)-generated makefiles for Visual Studio 2012 and gnu make can be found in the Build folder.

trying out
----------

You can try it out by cloning [this ideone code](http://ideone.com/kYcGJV).

formatting strings
------------------

Usage is extermely simple in case some prerequisites are met:
- you use STL for which there are default implementations and configurations
- the input parameters are `ofstream`-streamable
- you use single-byte character set

in terms of a test:
````cpp
REQUIRE(ff::format("{2}ff{1}").with('a').also_with(7).now()=="7ffa");
````

unlike the typical c++ string format libraries, `fakeformat` evaluates the parameters eagerly and saves them temporarily as strings. This allows for a split definition of parameters:
````cpp
  auto f=ff::format("ff{1}");
	REQUIRE(f.now()=="ff{1}");
	f.with(42);
	f.with(13);
	REQUIRE(f.now()=="ff42");
````

Unused parameters are ok.

If you need a slight modification of the language, a configuration is no big deal:

````cpp
struct test_config {
	static const char scope_begin='[';
	static const char scope_end=']';
	static const char separator=',';
	static const char equals='=';
	static const size_t index_begin=0;
	static bool string_to_key(std::string const& to_parse,int& res) {
		... parsing
		if (parsed...)
			return true;
		return false;
	}
};
//...
REQUIRE(ff::formatter<test_config>("[1][0]").with('a').also_with('b').now()=="ba");
````

The function `string_to_key` is used for parsing integers from strings. It is currently safe, but very inefficient. Feel free to modify that one. Check out [an article](http://www.kumobius.com/2013/08/c-string-to-int/) on converting strings to integers.  

format modifiers
----------------

Some format modifiers similar to [Boost.Locale](http://www.boost.org/doc/libs/1_52_0/libs/locale/doc/html/localized_text_formatting.html) are supported:
- `num` or `number` with parameters:
 - hex
 - oct
 - sci or scientific
 - fix or fixed
- `width` or `w`
 - number parameter required
- left
- right
- precision
 - number parameter required
- fill
 - a single character is allowed. Literal `}` is not supported yet

test
----

For the rest of the API, check out the [tests](https://github.com/d-led/fakeformat/tree/master/test).

status
------

Currently tested (Win32 and x64) with Visual Studio 2012 Express, and 32 bit in gcc 4.7.3, but should be no problem to test and fix on other compilers and platforms. A premake4 script should help with that task.

Testing is done using [Catch](https://github.com/philsquared/Catch).

structure
---------

Check out the [blog post](http://ledentsov.de/2013/09/21/fakeformat-ci/)

license
=======

Copyright 2013, Dmitry Ledentsov
MIT License: http://www.opensource.org/licenses/mit-license.php
