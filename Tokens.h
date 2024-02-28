#pragma once

#include <string>
#include <set>

#define INFINITE_REPEAT 0xFFFFFFFF

using std::string;
using std::set;

/**
 * Literal, Charset are values
 * Repeat is a suffix operator
 * Or, Edge are infix operators
*/
enum TokenType : char {
    Literal, Charset, Repeat, Or, OpenGroup, CloseGroup, Backref, Edge
};

enum EdgeType : char {
    StartWord, EndWord, IsEdge, NotEdge
};

/**
 * All tokens are immutable
*/
struct Token {
    TokenType type;

    Token(TokenType t) : type(t) { }
};

struct LiteralToken : public Token {
    string lit;

    LiteralToken(string& s) : Token(Literal), lit(s) { }
};

struct CharsetToken : public Token {
  public:
    CharsetToken(bool isNeg, const set<char>& chs);

    set<char> chars;
};

struct RepeatToken : public Token {
    unsigned int lowerBound, upperBound;

    RepeatToken(unsigned char lBound, unsigned char uBound) : Token(Repeat), lowerBound(lBound), upperBound(uBound) { }
};

/**
 * For open and close group, as well as backref
*/
struct GroupToken : public Token {
    unsigned char groupNumber;

    GroupToken(TokenType t, unsigned char num) : Token(t), groupNumber(num) { }
};

/**
 * For \b \B \< \>
*/
struct EdgeToken : public Token {
    EdgeType edgeType;

    EdgeToken(EdgeType et) : Token(Edge), edgeType(et) { }
};