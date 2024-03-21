#pragma once

#include <string>
#include <set>
#include <iostream>

#define INFINITE_REPEAT 0xFFFFFFFF

using std::string;
using std::set;
using std::ostream;

/**
 * Literal, Charset are values
 * Repeat is a suffix operator
 * Or is an infix operator
*/
enum TokenType : char {
    Literal, Charset, Repeat, Or, OpenGroup, CloseGroup, Backref
};

/**
 * All tokens are immutable
*/
struct Token {
    TokenType type;

    Token(TokenType t) : type(t) { }

    virtual string toString();
};

struct LiteralToken : public Token {
    string lit;

    LiteralToken(string& s) : Token(Literal), lit(s) { }
    LiteralToken(char c) : Token(Literal), lit(1, c) { }

    virtual string toString();
};

struct CharsetToken : public Token {
    set<char> chars;

    CharsetToken(bool isNeg, const set<char>& chs);

    virtual string toString();
};

struct RepeatToken : public Token {
    unsigned int lowerBound, upperBound;

    RepeatToken(unsigned int lBound, unsigned int uBound) : Token(Repeat), lowerBound(lBound), upperBound(uBound) { }

    virtual string toString();
};

/**
 * For open and close group, as well as backref
*/
struct GroupToken : public Token {
    unsigned char groupNumber;

    GroupToken(TokenType t, unsigned char num) : Token(t), groupNumber(num) { }

    virtual string toString();
};

inline ostream& operator<<(ostream& os, Token* t) {
    os << t->toString();;
    return os;
}