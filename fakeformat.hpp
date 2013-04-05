#pragma once

// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include <string>
#include <sstream>
#include <vector>

namespace ff {

	struct config {
		static const char scope_begin='{';
		static const char scope_end='}';
	};

	typedef config TConfig;

	namespace detail {
		
		template <
			typename TString=std::string,
			typename TStream=std::stringstream
			>
		class stream {

		public:
			stream ()
			{}

			stream (const stream &)
			{}

			stream & operator = (const stream &) {
				return *this;
			}

		public:
			template <typename T>
			stream& put(T const& p) {
				impl<<p;
				return *this;
			}

			TString str() const {
				return impl.str();
			}

			void clear() const {
				impl.str("");
			}

		private:
			typedef std::stringstream TStream;
			mutable TStream impl;
		};
	}

	typedef detail::stream<std::string,std::stringstream> TStream;
	typedef std::string TString;
	typedef std::string::const_iterator TPos;
	typedef std::string TParam;
	typedef std::vector<TParam> TParameters;

	class formatter {

	public:
		formatter(TString fmt):
			format_string(fmt)
		{
		}

	public:

		formatter& with(TParam const& param) {
			stream.clear();
			stream.put(param);
			parameters.push_back(stream.str());
			return *this;
		}

		formatter& and_with(TParam const& param) {
			return with(param);
		}

		TString now() const {
			size_t param_count=parameters.size();

			if (param_count==0) {
				return format_string;
			} else {
				return parse_and_format();
			}
		}

	private:

		TString parse_and_format() const {
			stream.clear();

			TPos beginning;
			for (TPos current=format_string.begin(); current!=format_string.end(); ++current) {
			}

			return stream.str();
		}
	
	private:
		TString format_string;
		TParameters parameters;
		TStream stream;
	};

	template <typename TFmt>
	formatter format(TFmt fmt) {
		return formatter(fmt);
	}
}