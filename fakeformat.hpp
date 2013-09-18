#pragma once

// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include <stdlib.h>
#include <stdexcept>

#include <iostream> //tmp

#ifndef FAKEFORMAT_NO_DEFAULTS
//configurable
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#endif

namespace ff {

	template <typename TChar>
	struct config {
		static const TChar scope_begin='{';
		static const TChar scope_end='}';
		static const TChar separator=',';
		static const size_t index_begin=1;
	};

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
			mutable TStream impl;
		};
	}


#ifndef FAKEFORMAT_NO_DEFAULTS
	typedef detail::stream<std::string,std::stringstream> TStream;
	typedef std::string TString;
	typedef size_t TPos;
	typedef std::string TParam;
	typedef std::vector<TParam> TParameters;
	typedef config<char> TConfig;
	typedef std::pair< int,std::map<TString,TString> > TFormatSpecifier;
	typedef std::map< std::pair<TPos,TPos>, TFormatSpecifier > TConfigPara;
#endif

	template <
	typename TConfig
	>
	class formatter {

	public:
		formatter(TString fmt):
		format_string(fmt)
		{
			parse_format_string();
			for (auto x: preparsed_format) {
				std::cout<<x.first.first<<":"<<x.second.first<<std::endl;
			}
		}

	public:

	// 	template <typename T>
	// 	formatter& with(T const& param) {
	// 		stream.clear();
	// 		stream.put(param);
	// 		parameters.push_back(stream.str());
	// 		return *this;
	// 	}

	// 	template <typename T>
	// 	formatter& also_with(T const& param) {
	// 		return with(param);
	// 	}

	// 	TString now() const {
	// 		size_t param_count=parameters.size();

	// 		if (param_count==0) {
	// 			return format_string;
	// 		} else {
	// 			return parse_and_format();
	// 		}
	// 	}

	// 	void clear_parameters() {
	// 		parameters.clear();
	// 	}

	// private:

	// 	TString parse_and_format() const {
	// 		stream.clear();

	// 		TPos last=format_string.begin();
	// 		for (TPos current=format_string.begin();
	// 			current!=format_string.end();
	// 			++current) {

	// 			// predicate can be refactored
	// 			if (*current==TConfig::scope_begin) {
	// 				stream.put(last,current);
	// 				last=current;
	// 				continue;
	// 			}

	// 			// predicate can be refactored
	// 			if (*current==TConfig::scope_end) {
	// 				TPos key_beginning=last;
	// 				++key_beginning;

	// 				int key=string_to_key(key_beginning,current);
	// 				if (valid_range(key)) {
	// 					stream.put(parameters[key]);
	// 					//peek
	// 					++current;
	// 					last=current;
	// 					if (current==format_string.end())
	// 						break;
	// 					else
	// 						continue;
	// 				} else {
	// 					stream.put(last,current);
	// 					last=current;
	// 					continue;
	// 				}
	// 			}
	// 		}

	// 		// finish writing
	// 		if (last!=format_string.end())
	// 			stream.put(last,format_string.end());

	// 		return stream.str();
	// 	}

	// 	bool valid_range(int key) const {
	// 		return key>=0 && (size_t)key<parameters.size();
	// 	}

		static int string_to_key(TString const& to_parse) {
			int res=-1;
			/* //c++11
			try {
				//configure?
				res=std::stoi(TString(beg,end))
				-TConfig::index_begin;
			} catch (std::exception&) {}
			*/
			res = atoi(to_parse.c_str());
			return res;
		}

	private:
		void parse_format_string() {
			TPos length=format_string.length();
			
			//state 
			enum State {
				GENERAL=0,
				COLLECTING_PLACEHOLDER,
				COLLECTING_KEY,
				COLLECTING_VALUE
			} state = GENERAL;

			size_t last_brace_pos=0;
			TConfigPara::mapped_type format_specifier;

			for (TPos pos = 0; pos < length; ++pos) {
				char cc=format_string[pos];

				if (state==GENERAL) {
					if (cc == TConfig::scope_begin) {
						state=COLLECTING_PLACEHOLDER;
						last_brace_pos=pos;
					}
				} else if (state==COLLECTING_PLACEHOLDER) {
					if (cc==TConfig::separator) {
						format_specifier.first=string_to_key(TString(format_string.begin()+last_brace_pos+1,format_string.begin()+pos));
						state=COLLECTING_KEY;
					} else if (cc==TConfig::scope_end) {
						preparsed_format[std::make_pair(last_brace_pos,pos+1-last_brace_pos)]=format_specifier;
						state=GENERAL;
					} else {
					}
				} else if (state == COLLECTING_KEY) {
					if (cc==TConfig::scope_end) {
						state=GENERAL;
						preparsed_format[std::make_pair(last_brace_pos,pos+1-last_brace_pos)]=format_specifier;
					}
				} else {
					
				}
			}
		}

	private:
		TString format_string;
		TConfigPara preparsed_format;
	// 	TParameters parameters;

	// 	TStream stream; // Caution: the stream is shared within the implementation!
	};

#ifndef FAKEFORMAT_NO_DEFAULTS
	formatter<TConfig> format(TString fmt) {
		return formatter<TConfig>(fmt);
	}

	template <typename TConfig>
	formatter<TConfig> format(TString fmt) {
		return formatter<TConfig>(fmt);
	}
#endif

}