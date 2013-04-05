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

TEST_CASE("format/one argument","one argument should be usable") {
	auto f=ff::format("ff{1}").with('a');
	REQUIRE(f.now()=="ffa");

	REQUIRE_NOTHROW(f.clear_parameters());

	REQUIRE(f.now()=="ff{1}");

	f.with(123);
	f.with(456);

	REQUIRE(f.now()=="ff123");
}

TEST_CASE("format/two arguments","two arguments should be usable") {
	auto f=ff::format("{2}ff{1}").with('a');
	REQUIRE(f.now()=="{2}ffa");
	
	f.with(7);

	REQUIRE(f.now()=="7ffa");

	f.clear_parameters();
	REQUIRE(f.now()=="{2}ff{1}");

	REQUIRE(f.with('a').also_with(7).now()=="7ffa");
}

namespace {
struct test_config {
	static const char scope_begin='[';
	static const char scope_end=']';
	static const size_t index_begin=0;
};
}

TEST_CASE("format/custom config","formatting should be somewhat configurable") {
	std::string fmt("[1][0]");
	
	REQUIRE(ff::format<ff::TDefaultConfig>(fmt).with('a').also_with('b').now()==fmt);

	REQUIRE(ff::format<::test_config>(fmt).with('a').also_with('b').now()=="ba");
}

TEST_CASE("format/border cases","the behavior should produce the least astonishment") {
	REQUIRE(ff::format("").now()=="");
	REQUIRE(ff::format(" ").now()==" ");
	REQUIRE(ff::format("{0}").with("a").now()=="{0}");
}