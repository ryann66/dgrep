#include <stdexcept>

#include "Tokens.h"
#include "globals.h"

using std::set;
using std::range_error;
using std::to_string;

string Token::toString() {
    if (type == Or) return "Or";
    return "Invalid";
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