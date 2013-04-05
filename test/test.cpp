// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include "../fakeformat.hpp"

#include <string>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

ANON_TEST_CASE() {
	auto empty=ff::format("");
	REQUIRE(empty.now()=="");

	auto f=ff::format("bla");
	REQUIRE(f.now()=="bla");

	std::string fmt("flim");
	auto ff=ff::format(fmt);
	auto fff=ff::format(fmt.c_str());
	REQUIRE(ff.now()==fmt);
	REQUIRE(fff.now()==fmt);
}