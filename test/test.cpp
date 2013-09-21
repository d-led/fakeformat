// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include "../fakeformat.hpp"

#include <string>
#include <sstream>
#include <iomanip>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE() {
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
	static const char separator=',';
	static const char equals='=';
	static const size_t index_begin=0;
};
}

TEST_CASE("format/custom config","formatting should be somewhat configurable") {
	std::string fmt("[1][0]");
	
	REQUIRE(ff::format(fmt).with('a').also_with('b').now()==fmt);

	REQUIRE(ff::formatter<test_config>(fmt).with('a').also_with('b').now()=="ba");
}

TEST_CASE("format/border cases","the behavior should produce the least astonishment") {
	REQUIRE(ff::format("").now()=="");
	REQUIRE(ff::format(" ").now()==" ");
	REQUIRE(ff::format("{0}").with("a").now()=="{0}");
}

TEST_CASE("placeholder parsing is strict without whitespaces") {
	REQUIRE(ff::format("{1x}").with("a").now()=="{1x}");
	REQUIRE(ff::format("{x1}").with("a").now()=="{x1}");
	REQUIRE(ff::format("{ 01 }").with("a").now()=="{ 01 }");
	REQUIRE(ff::format("{ 0 1 }").with("a").now()=="{ 0 1 }");
}

TEST_CASE("formatting numbers as hex") {
	REQUIRE(ff::format("{1}{1,number=hex}{1}{1,num=hex}").with(10).now()=="10a10a");
}

TEST_CASE("formatting numbers as oct") {
	REQUIRE(ff::format("{1}{1,number=oct}{1}{1,num=oct}").with(10).now()=="10121012");
}

TEST_CASE("formatting numbers as scientific") {
    std::stringstream ss;
    ss<<1.1;
    ss<<std::scientific<<1.1;
    ss<<"1.1";
    ss<<1.1;
	REQUIRE(ff::format("{1}{1,number=sci}{1}{1,num=scientific}").with(1.1).now()==ss.str());
}

TEST_CASE("formatting numbers as fixed") {
	REQUIRE(ff::format("{1}{1,number=fix}{1}{1,num=fixed}").with(1.1).now()=="1.11.1000001.11.100000");
}

TEST_CASE("setting width") {
	REQUIRE(ff::format("{1}{1,width=3}{1}{1,w=0}").with(1).now()=="1  111");
}

TEST_CASE("setting alignment") {
	REQUIRE(ff::format("{1,width=3,left}{1,right,width=3}").with(1).now()=="1    1");
}

TEST_CASE("setting precision") {
	REQUIRE(ff::format("{1,num=fixed,precision=3}{1}").with(1.0).now()=="1.0001");
}
