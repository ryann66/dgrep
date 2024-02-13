#pragma once

#include <string>
#include <set>

#define INFINITE_REPEAT 0xFF

using std::string;
using std::set;

/**
 * Literal, Charset are values
 * Repeat is a suffix operator
 * Or, Concat are infix operators
*/
enum TokenType : char {
    Literal, Charset, Repeat, Or, Concat, OpenGroup, CloseGroup
};

struct Token {
    TokenType type;
};

struct LiteralToken : public Token {
    string lit;
};

struct CharsetToken : public Token {
    bool isNegation;
    set<char> chars;
};

struct RepeatToken : public Token {
    unsigned char UpperBound, LowerBound;
};

struct OpenGroupToken : public Token {
    unsigned char GroupNumber;
};