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

	class formatter {
		typedef std::string TString;
		typedef std::string TParam;
		typedef std::vector<TParam> TParameters;
		typedef std::stringstream TStream;
		typedef config TConfig;

	public:
		formatter(TString fmt):
			format_string(fmt)
		{
		}

	public:

		formatter& With(TParam param) {
			parameters.push_back(param);
		}

		TString Now() const {
			size_t param_count=parameters.size();

			if (param_count==0) {
				return format_string;
			} else {
				TStream stream;
				return stream.str();
			}
		}
	
	private:
		TString format_string;
		TParameters parameters;
	};

	template <typename TFmt>
	formatter format (TFmt fmt) {
		return formatter(fmt);
	}
}