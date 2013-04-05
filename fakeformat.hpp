#pragma once

// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include <string>
#include <sstream>
#include <vector>

namespace ff {

	class formatter {
		typedef std::string TString;
		typedef std::vector<TString> TParameters;
		typedef std::stringstream TStream;

	public:
		formatter(TString fmt):
			format_string(fmt)
		{
		}
	
	private:
		TString format_string;
	};

	template <typename TFmt>
	formatter format (TFmt fmt) {
		return formatter(fmt);
	}
}