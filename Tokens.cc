#include <stdexcept>
#include <cstring>

#include "Tokens.h"
#include "globals.h"

using std::set;
using std::range_error;
using std::to_string;

string Token::toString() {
    if (type == Or) return "Or";
    return "Invalid";
}

LiteralToken::LiteralToken(string& s) : Token(Literal), lit(s) {
    const char* str = s.c_str();
    while (*str) {
        if (!alphabet.contains(*str)) throw new range_error("'" + string(1,*str) + "' is not in alphabet");
        str++;
    }
}

LiteralToken::LiteralToken(char c) : Token(Literal), lit(1, c) {
    if (!alphabet.contains(c)) throw new range_error("'" + string(1,c) + "' is not in alphabet");
}

string LiteralToken::toString() {
    if (type == Literal) return "Literal: " + lit;
    return "Invalid";
}

string CharsetToken::toString() {
    if (type != Charset) return "Invalid";
    string s("Charset: ");
    s.reserve(s.size() + chars.size());
    for (char c : chars) {
        s.append(1, c);
    }
    return s;
}

string RepeatToken::toString() {
    if (type == Repeat) return "Repeat: " + to_string(lowerBound) + " - " + to_string(upperBound);
    return "Invalid";
}

string GroupToken::toString() {
    if (type == OpenGroup) return "OpenGroup: " + to_string((int) groupNumber);
    else if (type == CloseGroup) return "CloseGroup: " + to_string((int) groupNumber);
    else if (type == Backref) return "Backref: " + to_string((int) groupNumber);
    return "Invalid";
}