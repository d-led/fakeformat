// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#define FAKEFORMAT_NO_DEFAULTS
#include "../fakeformat.hpp"

#include <string>
#include <vector>
#include <catch.hpp>

namespace {

	class simple_stream {
		public:
			template <typename T>
			simple_stream const& put(T const& p) const {
				impl+=p;
				return *this;
			}

			template <typename TIter>
			simple_stream const& put(TIter first,TIter last) const {
				impl+=std::string(first,last);
				return *this;
			}

			std::string str() const {
				return impl;
			}

			void clear() const {
				impl="";
			}

		private:
			mutable std::string impl;
		};

}

TEST_CASE("format/no defaults","STL headers should be avoidable") {
	using std::string;
	using std::vector;
	auto f=ff::format<ff::config<char>,::simple_stream,string,string::const_iterator,string,vector<string>>("{2}{1}");
	f.with('a').also_with("b");
	REQUIRE(f.now()=="ba");
}