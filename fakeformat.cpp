#include "fakeformat.hpp"
namespace ff {
//----------------------------------------------
// format.cpp
// FSM:       FormatParser
// Context:   FormatContext
// Version:   
// Generated: Samstag 09/21/2013 at 16:10:38 MESZ
//


static char _versID[]  = "";



//----------------------------------------------
// Definitions of static state objects

//----------------------------------------------
FormatParserReadingKeyState FormatParser::ReadingKey;
FormatParserGeneralState FormatParser::General;
FormatParserReadingPlaceholderState FormatParser::ReadingPlaceholder;
FormatParserReadingValueState FormatParser::ReadingValue;

//----------------------------------------------
// Default Event Functions

//----------------------------------------------

void FormatParserState::ReadEqualsSign(FormatParser& s)
  { s.FSMError("ReadEqualsSign", s.GetState().StateName()); }

void FormatParserState::ReadRightBrace(FormatParser& s)
  { s.FSMError("ReadRightBrace", s.GetState().StateName()); }

void FormatParserState::ReadLeftBrace(FormatParser& s)
  { s.FSMError("ReadLeftBrace", s.GetState().StateName()); }

void FormatParserState::ReadComma(FormatParser& s)
  { s.FSMError("ReadComma", s.GetState().StateName()); }

//----------------------------------------------
// The States and their Transitions
//----------------------------------------------

//----------------------------------------------
// ReadingKey Actions and Transitions
//----------------------------------------------

// Starting State: ReadingKey
// Event:          ReadComma
//
void FormatParserReadingKeyState::ReadComma( FormatParser& s )
{

    s.AddKey();
    s.ContinueCollectingKeys();

    // Change the state
    s.SetState(FormatParser::ReadingKey);
}

// Starting State: ReadingKey
// Event:          ReadLeftBrace
//
void FormatParserReadingKeyState::ReadLeftBrace( FormatParser& s )
{

    s.StartCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::ReadingPlaceholder);
}

// Starting State: ReadingKey
// Event:          ReadEqualsSign
//
void FormatParserReadingKeyState::ReadEqualsSign( FormatParser& s )
{

    s.AddKey();
    s.StartAddingValue();

    // Change the state
    s.SetState(FormatParser::ReadingValue);
}

// Starting State: ReadingKey
// Event:          ReadRightBrace
//
void FormatParserReadingKeyState::ReadRightBrace( FormatParser& s )
{

    s.FinishCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::General);
}

//----------------------------------------------
// General Actions and Transitions
//----------------------------------------------

// Starting State: General
// Event:          ReadLeftBrace
//
void FormatParserGeneralState::ReadLeftBrace( FormatParser& s )
{

    s.StartCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::ReadingPlaceholder);
}

//----------------------------------------------
// ReadingPlaceholder Actions and Transitions
//----------------------------------------------

// Starting State: ReadingPlaceholder
// Event:          ReadRightBrace
//
void FormatParserReadingPlaceholderState::ReadRightBrace( FormatParser& s )
{

    s.ParsePlaceholder();
    s.FinishCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::General);
}

// Starting State: ReadingPlaceholder
// Event:          ReadComma
//
void FormatParserReadingPlaceholderState::ReadComma( FormatParser& s )
{

    s.ParsePlaceholder();
    s.StartKey();

    // Change the state
    s.SetState(FormatParser::ReadingKey);
}

// Starting State: ReadingPlaceholder
// Event:          ReadLeftBrace
//
void FormatParserReadingPlaceholderState::ReadLeftBrace( FormatParser& s )
{

    s.StartCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::ReadingPlaceholder);
}

//----------------------------------------------
// ReadingValue Actions and Transitions
//----------------------------------------------

// Starting State: ReadingValue
// Event:          ReadComma
//
void FormatParserReadingValueState::ReadComma( FormatParser& s )
{

    s.AddValue();

    // Change the state
    s.SetState(FormatParser::ReadingKey);
}

// Starting State: ReadingValue
// Event:          ReadRightBrace
//
void FormatParserReadingValueState::ReadRightBrace( FormatParser& s )
{

    s.AddValue();
    s.FinishCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::General);
}

// Starting State: ReadingValue
// Event:          ReadLeftBrace
//
void FormatParserReadingValueState::ReadLeftBrace( FormatParser& s )
{

    s.StartCollectingPlaceholder();

    // Change the state
    s.SetState(FormatParser::ReadingPlaceholder);
}

//----------------------------------------------
// State Machine Constructor: FormatParser
//  set Initial State to: General
//
//----------------------------------------------
FormatParser::FormatParser() : itsState(&General)
{
    // Entry functions for: General
}

//  Get version information
//
const char* FormatParser::GetVersion() const
{ return _versID; }

}
