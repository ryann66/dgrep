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
    Literal, Charset, Repeat, Or, Concat, OpenGroup, CloseGroup, Backref, Edge
};

enum EdgeType : char {
    StartWord, EndWord, Edge, NotEdge
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

/**
 * For open and close group, as well as backref
*/
struct GroupToken : public Token {
    unsigned char GroupNumber;
};

/**
 * For \b \B \< \>
*/
struct EdgeToken : public Token {
    EdgeType edgeType;
};