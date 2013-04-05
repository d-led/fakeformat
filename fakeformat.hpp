#pragma once

// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include <stdlib.h>
#include <stdexcept>

//configurable
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace ff {

	typedef char TChar;

	struct config {
		static const char scope_begin='{';
		static const char scope_end='}';
		static const size_t index_begin=1;
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
			stream const& put(T const& p) const {
				impl<<p;
				return *this;
			}

			template <typename TIter>
			stream const& put(TIter first,TIter last) const {
				for (TIter it=first; it!=last; ++it)
					impl.put(*it);
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

		template <typename T>
		formatter& with(T const& param) {
			stream.clear();
			stream.put(param);
			parameters.push_back(stream.str());
			return *this;
		}

		template <typename T>
		formatter& also_with(T const& param) {
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

		void clear_parameters() {
			parameters.clear();
		}

	private:

		TString parse_and_format() const {
			stream.clear();

			TPos last=format_string.begin();
			for (TPos current=format_string.begin();
				current!=format_string.end();
				++current) {

				// predicate can be refactored
				if (*current==TConfig::scope_begin) {
					stream.put(last,current);
					last=current;
					continue;
				}

				// predicate can be refactored
				if (*current==TConfig::scope_end) {
					TPos key_beginning=last;
					++key_beginning;
					
					int key=string_to_key(key_beginning,current);
					if (valid_range(key)) {
						stream.put(parameters[key]);
						//peek
						++current;
						last=current;
						if (current==format_string.end())
							break;
						else
							continue;
					} else {
						stream.put(last,current);
						last=current;
						continue;
					}
				}
			}

			// finish writing
			if (last!=format_string.end())
				stream.put(last,format_string.end());

			return stream.str();
		}

		bool valid_range(int key) const {
			return key>=0 && (size_t)key<parameters.size();
		}

		static int string_to_key(TPos beg,TPos end) {
			int res=-1;
			try {
				//configure?
				res=std::stoi(TString(beg,end))
					-TConfig::index_begin;
			} catch (std::exception&) {}
			return res;
		}
	
	private:
		TString format_string;
		TParameters parameters;

		TStream stream; // Caution: the stream is shared within the implementation!
	};

	template <typename TFmt>
	formatter format(TFmt fmt) {
		return formatter(fmt);
	}
}