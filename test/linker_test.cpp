#include "../fakeformat.hpp"

#include <catch.hpp>

TEST_CASE("since the library is header only, it should be checked, whether multiple instances are ok") {
	CHECK(ff::format("{1}").with(1).now()=="1");
}
