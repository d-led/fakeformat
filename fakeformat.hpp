#pragma once

// Copyright 2013, Dmitry Ledentsov
// Released under the MIT License: http://www.opensource.org/licenses/mit-license.php

#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <iomanip>

namespace ff {

	template <typename TChar>
	struct config {
		static const TChar scope_begin='{';
		static const TChar scope_end='}';
		static const TChar separator=',';
		static const TChar equals='=';
		static const size_t index_begin=1;
		static bool string_to_key(std::string const& to_parse,int& res) {
			std::istringstream ss(to_parse);
			ss.imbue(std::locale::classic());
			ss >> res;
			if (!ss.fail() && ss.eof())
				return true;
			return false;
		}
	};

	typedef std::map<std::string,std::string> TKeyValueMap;

	namespace detail {
		
		template <
		typename TString=std::string,
		typename TStream=std::stringstream
		>
		class stream {

		public:
			stream ()
			{
				flags=impl.flags();
				impl.imbue(std::locale());
			}

			stream (const stream &)
			{
				flags=impl.flags();
				impl.imbue(std::locale());
			}

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
				impl.flags(flags);
			}

			void configure(TKeyValueMap const& config) {
				for (TKeyValueMap::const_iterator it=config.begin();
					 it!=config.end();
					 ++it) {
						 if (it->first=="num"
							 || it->first=="number") {
								 if (it->second=="hex") {
									 impl<<std::hex;
								 } else if (it->second=="oct") {
									 impl<<std::oct;
								 } else if (it->second=="sci"
											|| it->second=="scientific") {
									 impl<<std::scientific;
								 } else if (it->second=="fix"
											|| it->second=="fixed") {
									 impl<<std::fixed;
								 }
						 } else if (it->first=="w"
									|| it->first=="width") {
										int width=-1;
										if (TConfig::string_to_key(it->second,width)
											&& width>0) {
												impl.width(width);
										}
						 } else if (it->first=="left") {
							 impl<<std::left;
						 } else if (it->first=="right") {
							 impl<<std::right;
						 }
				}
			}

		private:
			mutable TStream impl;
			int flags;
		};
	}

	//////////////////PARSER BEGIN////////////////////////

static const int START_POS=-1;

struct Placeholder {
	int id;
	int length;
	TKeyValueMap config;
	Placeholder():id(START_POS),length(START_POS){}
};

typedef std::vector< std::pair<int,Placeholder> > Placeholders;

class FormatContext
{
	struct ParserState {
		std::string format_string;
		int pos;
		int last_left_brace;
		int last_key_start;
		int last_value_start;
		std::string last_key;
		Placeholder current_placeholder;
		Placeholders placeholders;
	} state;

	int string_to_key(std::string const& to_parse) {
		int res=START_POS;
		std::istringstream ss(to_parse);
		ss.imbue(std::locale::classic());
		ss >> res;
		if (!ss.fail() && ss.eof())
			return res;
		return START_POS;
	}

public:

	bool IsAtEnd() const {
		bool res=state.pos >= (static_cast<int>(state.format_string.size())-1);
		return res;
	}

	char Step() {
		if (IsAtEnd())
			return '\0';

		state.pos++;
		return state.format_string[state.pos];
	}

	void ResetPlaceholderState() {
		state.last_value_start=state.last_key_start=state.last_left_brace=START_POS;
	}
	
	void SetString(std::string const& _) { 
		state.format_string=_;
		state.pos=START_POS;
		ResetPlaceholderState();
	}

	void TryAddPlaceholder() {
		if (state.current_placeholder.id!=START_POS) {
			state.current_placeholder.length=state.pos-state.last_left_brace+1;
			state.placeholders.push_back(std::make_pair(state.last_left_brace,state.current_placeholder));
		}
	}

public:


	void StartCollectingPlaceholder() {
		state.current_placeholder=Placeholder();
		state.last_left_brace=state.pos;
		state.last_key_start=state.last_value_start=START_POS;
		state.last_key="";
	}

	void ParsePlaceholder() {
		int id=string_to_key(std::string(state.format_string.begin()+state.last_left_brace+1,state.format_string.begin()+state.pos));
		state.current_placeholder.id=id;
	}

	void AddValue() {
		state.current_placeholder.config[state.last_key]=
			std::string(state.format_string.begin()+state.last_value_start+1,state.format_string.begin()+state.pos);
		state.last_key_start=state.pos;
		state.last_value_start=START_POS;
	}

	void StartKey() {
		state.last_key_start=state.pos;
		state.last_value_start=START_POS;
		state.last_key="";
	}

	void AddKey() {
		if ( (state.pos-state.last_key_start) < 1 )
			return;
		state.last_key=std::string(state.format_string.begin()+state.last_key_start+1,state.format_string.begin()+state.pos);
		state.current_placeholder.config[state.last_key]="";
	}

	void ContinueCollectingKeys() {
		state.last_key_start=state.pos;
	}

	void StartAddingValue() {
		state.last_key_start=START_POS;
		state.last_value_start=state.pos;
	}

	void FinishCollectingPlaceholder() {
		if (state.last_key_start!=START_POS)
			AddKey();

		if (state.last_left_brace!=START_POS
			&& (state.pos-state.last_left_brace) > 1) {
				TryAddPlaceholder();
		}

		ResetPlaceholderState();
	}

	void Continue() {
	}

	void FSMError(const char*, const char*) {
	}

	Placeholders Get() {
		return state.placeholders;
	}


};


// Forward Declarations

class FormatParser;

//----------------------------------------------
// FormatParserState: The base state class
//----------------------------------------------
class FormatParserState
{
  public: 
    virtual const char* StateName() const = 0;
    virtual void ReadEqualsSign( FormatParser& );
    virtual void ReadRightBrace( FormatParser& );
    virtual void ReadLeftBrace( FormatParser& );
    virtual void ReadComma( FormatParser& );
};

//----------------------------------------------
// State: ReadingKey
//----------------------------------------------
class FormatParserReadingKeyState : public FormatParserState
{
  public: 
    virtual const char* StateName() const
        { return "ReadingKey"; }
    virtual void ReadEqualsSign( FormatParser& );
    virtual void ReadComma( FormatParser& );
    virtual void ReadLeftBrace( FormatParser& );
    virtual void ReadRightBrace( FormatParser& );
};
//----------------------------------------------
// State: General
//----------------------------------------------
class FormatParserGeneralState : public FormatParserState
{
  public: 
    virtual const char* StateName() const
        { return "General"; }
    virtual void ReadLeftBrace( FormatParser& );
};
//----------------------------------------------
// State: ReadingPlaceholder
//----------------------------------------------
class FormatParserReadingPlaceholderState : public FormatParserState
{
  public: 
    virtual const char* StateName() const
        { return "ReadingPlaceholder"; }
    virtual void ReadLeftBrace( FormatParser& );
    virtual void ReadRightBrace( FormatParser& );
    virtual void ReadComma( FormatParser& );
};
//----------------------------------------------
// State: ReadingValue
//----------------------------------------------
class FormatParserReadingValueState : public FormatParserState
{
  public: 
    virtual const char* StateName() const
        { return "ReadingValue"; }
    virtual void ReadRightBrace( FormatParser& );
    virtual void ReadLeftBrace( FormatParser& );
    virtual void ReadComma( FormatParser& );
};
//----------------------------------------------
// FormatParser: The Finite State Machine class
//----------------------------------------------
class FormatParser: public FormatContext
{
  public: 
    // Static State variables
    static FormatParserReadingKeyState ReadingKey;
    static FormatParserGeneralState General;
    static FormatParserReadingPlaceholderState ReadingPlaceholder;
    static FormatParserReadingValueState ReadingValue;

    FormatParser(); // default Constructor

    // Event functions
    virtual void ReadEqualsSign() { itsState->ReadEqualsSign( *this ); }
    virtual void ReadRightBrace() { itsState->ReadRightBrace( *this ); }
    virtual void ReadLeftBrace() { itsState->ReadLeftBrace( *this ); }
    virtual void ReadComma() { itsState->ReadComma( *this ); }

    // State Accessor functions
    void SetState( FormatParserState& theState ) { itsState = &theState; }
    FormatParserState& GetState() const { return *itsState; }

    const char* GetCurrentStateName() const { return itsState->StateName(); }
    const char* GetVersion() const;

  private: 
    FormatParserState* itsState;
};

	//////////////////PARSER END//////////////////////////


	typedef detail::stream<std::string,std::stringstream> TStream;
	typedef std::string TString;
	typedef size_t TPos;
	typedef std::string TParam;
	typedef std::map<TPos,TParam> TParameters;
	typedef config<char> TConfig;

	template <
	typename TConfig
	>
	class formatter {

	public:
		formatter(TString fmt):
		format_string(fmt),
		pos(TConfig::index_begin)
		{
			parse_format_string();
		}

	public:

		template <typename T>
		formatter& with(T const& param) {
			size_t count=placeholders.size();
			for (size_t it=0;
				it<count;
				it++) {
					stream.clear();
					if (placeholders[it].second.id==pos) {
						stream.configure(placeholders[it].second.config);
						stream.put(param);
						parameters[it]=stream.str();
					}
			}
			pos++;
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
	 			return assemble_string();
	 		}
	 	}

	 	void clear_parameters() {
	 		parameters.clear();
			pos=TConfig::index_begin;
	 	}

	 private:

	 	TString assemble_string() const {
			std::string res(format_string);

			for (TParameters::const_reverse_iterator it=parameters.rbegin();
				 it!=parameters.rend();
				 ++it) {
					 std::pair<int,Placeholder> const& pl=placeholders[it->first];
					 res.replace(pl.first,pl.second.length,it->second);
			}

			return res;
	 	}

	private:
		void parse_format_string() {
			FormatParser f;
			f.SetString(format_string);	

			while (!f.IsAtEnd()) {
				char c=f.Step();
				switch (c) {
				case TConfig::scope_begin: f.ReadLeftBrace(); break;
				case TConfig::scope_end: f.ReadRightBrace(); break;
				case TConfig::separator: f.ReadComma(); break;
				case TConfig::equals: f.ReadEqualsSign(); break;
				default : f.Continue(); break;
				}
			}
			placeholders=f.Get();
		}

	private:
		TString format_string;
		Placeholders placeholders;
		TParameters parameters;
		TPos pos;

	 	TStream stream; // Caution: the stream is shared within the implementation!
	};

	typedef formatter< config<char> > format;

}
