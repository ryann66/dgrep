#include <cstdlib>
#include <cerrno>
#include <algorithm>
#include <stack>
#include <stdexcept>

#include "Tokenizer.h"
#include "syntax_error.h"
#include "globals.h"
#include "Alphabet.h"

using std::find;
using std::vector;
using std::string;
using std::set;
using std::stack;
using std::runtime_error;

void addLower(stack<char>& s) {
    for (char c = 'a'; c <= 'z'; c++) {
        s.push(c);
    }
}

void addUpper(stack<char>& s) {
    for (char c = 'A'; c <= 'Z'; c++) {
        s.push(c);
    }
}

void addDigits(stack<char>& s) {
    for (char c = '0'; c <= '9'; c++) {
        s.push(c);
    }
}

void addPunct(stack<char>& s) {
    for (char c = '!'; c <= '/'; c++) {
        s.push(c);
    }
    for (char c = ':'; c <= '@'; c++) {
        s.push(c);
    }
    for (char c = '['; c <= '`'; c++) {
        s.push(c);
    }
    for (char c = '{'; c <= '~'; c++) {
        s.push(c);
    }
}

void addSpace(stack<char>& s) {
    /*for (char c = 9; c <= 13; c++) {
        s.push(c);
    }*/
    s.push(' ');
}

vector<Token*> tokenize(string& str, set<unsigned char>* backrefs) {
    vector<Token*> tokens;
    string lit;
    const char* input = str.c_str();
    unsigned char maxGroupNum = 0;
    vector<unsigned char> openGroups;  // iterable stack
    try {
        while (*input) {
            switch(*input) {
                case '(':  // open
                    if (!lit.empty()) {
                        tokens.push_back(new LiteralToken(lit));
                        lit.clear();
                    }
                    tokens.push_back(new GroupToken(OpenGroup, ++maxGroupNum));
                    openGroups.push_back(maxGroupNum);
                    break;
                case ')':  // close
                    if (openGroups.empty()) {
                        // ERROR unmatched )
                        throw syntax_error("Unmatched )");
                    }
                    if (!lit.empty()) {
                        tokens.push_back(new LiteralToken(lit));
                        lit.clear();
                    }
                    tokens.push_back(new GroupToken(CloseGroup, openGroups.back()));
                    openGroups.pop_back();
                    break;
                case '{':  // arb repeat start
                    if (!lit.empty()) {
                        size_t s = lit.size();
                        if (s > 1) {
                            char c = lit[s-1];
                            lit.erase(s-1, 1);
                            tokens.push_back(new LiteralToken(lit));
                            tokens.push_back(new LiteralToken(c));
                        }
                        else {
                            tokens.push_back(new LiteralToken(lit));
                        }
                        lit.clear();
                    }
                    input++;
                    // either number or comma
                    long num;
                    // {,num}
                    if (*input == ',') {
                        input++;
                        num = strtol(input, const_cast<char**>(&input), 10);
                        if (num == 0 && errno == EINVAL) {
                            // ERROR {, must be followed by a number
                            throw syntax_error("{, must be followed by a number");
                        }
                        if (num < 0) {
                            // ERROR negative argument
                            throw syntax_error("Negative argument");
                        }
                        else if (num > INFINITE_REPEAT) {
                            // ERROR argument is too large
                            throw syntax_error("Argument too large");
                        }
                        if (*input != '}') {
                            // ERROR expected }
                            throw syntax_error("} expected");
                        }
                        tokens.push_back(new RepeatToken(0, num));
                        break;
                    }
                    num = strtol(input, const_cast<char**>(&input), 10);
                    if (num == 0 && errno == EINVAL) {
                        // ERROR { must be followed by a number or comma
                        throw syntax_error("{ must be followed by a number or comma");
                        break;
                    }
                    if (num < 0) {
                        // ERROR negative argument
                        throw syntax_error("Negative argument");
                    }
                    else if (num > INFINITE_REPEAT) {
                        // ERROR argument is too large
                        throw syntax_error("Argument too large");
                    }
                    // {num}
                    if (*input == '}') {
                        tokens.push_back(new RepeatToken(num, num));
                        break;
                    }
                    if (*input == ',') {
                        input++;
                        // {num,}
                        if (*input == '}') {
                            tokens.push_back(new RepeatToken(num, INFINITE_REPEAT));
                            break;
                        }
                        // {num,num}
                        long num2 = strtol(input, const_cast<char**>(&input), 10);
                        if (num2 == 0 && errno == EINVAL) {
                            // ERROR {num, must be followed by a number or }
                            throw syntax_error("{<num>, must be followed by a number of }");
                            break;
                        }
                        if (num2 < 0) {
                            // ERROR negative argument
                            throw syntax_error("Negative argument");
                        }
                        else if (num2 > INFINITE_REPEAT) {
                            // ERROR argument is too large
                            throw syntax_error("Argument too large");
                        }
                        if (num2 < num) {
                            // ERROR second argument must be greater than or equal to first argument
                            throw syntax_error("First argument exceeds second argument");
                        }
                        if (*input == '}') {
                            tokens.push_back(new RepeatToken(num, num2));
                            break;
                        }
                        // ERROR expected } after {num,num
                        throw syntax_error("{<num>,<num> must be followed by }");
                        input--;
                        break;
                    }
                    // ERROR {num expected to be followed by , or }
                    throw syntax_error("{<num> must be followed by , or }");
                    input--;
                    break;
                case '[':{  // charset start
                    if (!lit.empty()) {
                        tokens.push_back(new LiteralToken(lit));
                        lit.clear();
                    }
                    Alphabet* a = readCharset(&input);
                    tokens.push_back(new CharsetToken(*a));
                    delete a;
                    break;}
                case '|':  // or
                    if (!lit.empty()) {
                        tokens.push_back(new LiteralToken(lit));
                        lit.clear();
                    }
                    tokens.push_back(new Token(Or));
                    break;
                case '.':  // wildcard
                    if (!lit.empty()) {
                        tokens.push_back(new LiteralToken(lit));
                        lit.clear();
                    }
                    {
                        Alphabet a;
                        tokens.push_back(new CharsetToken(a));
                    }
                    break;
                case '?':  // zero or one
                    if (!lit.empty()) {
                        size_t s = lit.size();
                        if (s > 1) {
                            char c = lit[s-1];
                            lit.erase(s-1, 1);
                            tokens.push_back(new LiteralToken(lit));
                            tokens.push_back(new LiteralToken(c));
                        }
                        else {
                            tokens.push_back(new LiteralToken(lit));
                        }
                        lit.clear();
                    }
                    tokens.push_back(new RepeatToken(0, 1));
                    break;
                case '*':  // any number
                    if (!lit.empty()) {
                        size_t s = lit.size();
                        if (s > 1) {
                            char c = lit[s-1];
                            lit.erase(s-1, 1);
                            tokens.push_back(new LiteralToken(lit));
                            tokens.push_back(new LiteralToken(c));
                        }
                        else {
                            tokens.push_back(new LiteralToken(lit));
                        }
                        lit.clear();
                    }
                    tokens.push_back(new RepeatToken(0, INFINITE_REPEAT));
                    break;
                case '+':  // at least one
                    if (!lit.empty()) {
                        size_t s = lit.size();
                        if (s > 1) {
                            char c = lit[s-1];
                            lit.erase(s-1, 1);
                            tokens.push_back(new LiteralToken(lit));
                            tokens.push_back(new LiteralToken(c));
                        }
                        else {
                            tokens.push_back(new LiteralToken(lit));
                        }
                        lit.clear();
                    }
                    tokens.push_back(new RepeatToken(1, INFINITE_REPEAT));
                    break;
                case '\\':
                    input++;
                    switch(*input) {
                        case 'w':
                            // alpha or space
                            if (!lit.empty()) {
                                tokens.push_back(new LiteralToken(lit));
                                lit.clear();
                            }
                            {
                                stack<char> s;
                                addDigits(s);
                                addLower(s);
                                addUpper(s);
                                set<char> st;
                                st.insert('_');
                                while (!s.empty()) {
                                    st.insert(s.top());
                                    s.pop();
                                }
                                Alphabet a(false, st);
                                tokens.push_back(new CharsetToken(a));
                            }
                            break;
                        case 'W':
                            // not alpha or space
                            if (!lit.empty()) {
                                tokens.push_back(new LiteralToken(lit));
                                lit.clear();
                            }
                            {
                                stack<char> s;
                                addDigits(s);
                                addLower(s);
                                addUpper(s);
                                set<char> st;
                                st.insert('_');
                                while (!s.empty()) {
                                    st.insert(s.top());
                                    s.pop();
                                }
                                Alphabet a(true, st);
                                tokens.push_back(new CharsetToken(a));
                            }
                            break;
                        case 's':
                            // space
                            if (!lit.empty()) {
                                tokens.push_back(new LiteralToken(lit));
                                lit.clear();
                            }
                            {
                                stack<char> s;
                                addSpace(s);
                                set<char> st;
                                while (!s.empty()) {
                                    st.insert(s.top());
                                    s.pop();
                                }
                                Alphabet a(false, st);
                                tokens.push_back(new CharsetToken(a));
                            }
                            break;
                        case 'S':
                            // non space
                            if (!lit.empty()) {
                                tokens.push_back(new LiteralToken(lit));
                                lit.clear();
                            }
                            {
                                stack<char> s;
                                addSpace(s);
                                set<char> st;
                                while (!s.empty()) {
                                    st.insert(s.top());
                                    s.pop();
                                }
                                Alphabet a(true, st);
                                tokens.push_back(new CharsetToken(a));
                            }
                            break;
                        case '*':
                        case '+':
                        case '?':
                        case '.':
                        case '{':
                        case '[':
                        case '(':
                        case '}':
                        case ']':
                        case ')':
                        case '|':
                        case '\\':
                        // grep compat (allow escaping of noncontrol characters)
                        case '^':
                        case '$':
                            lit.push_back(*input);
                            break;
                        case '\0':
                            // ERROR trailing escape
                            throw syntax_error("Trailing \\");
                        default:
                            if (*input > 48 && *input < 58) {
                                // digit (backref)
                                unsigned char n = *input - 48;
                                if (n > maxGroupNum || find(openGroups.begin(), openGroups.end(), n) != openGroups.end()) {
                                    // ERROR group is currently open
                                    throw syntax_error("Invalid backref (currently open)");
                                }
                                if (!lit.empty()) {
                                    tokens.push_back(new LiteralToken(lit));
                                    lit.clear();
                                }
                                tokens.push_back(new GroupToken(Backref, n));
                                backrefs->insert(n);
                                break;
                            }
                            // ERROR unknown escape character
                            throw syntax_error("Unknown escape character");
                    }
                    break;
                default:
                    lit.push_back(*input);
                    break;
            }
            input++;
        }

        if (!openGroups.empty()) {
            // ERROR unmatched (
            throw syntax_error("Unmatched (");
        }
        if (!lit.empty()) {
            tokens.push_back(new LiteralToken(lit));
            lit.clear();
        }
        
        return tokens;
    } catch (runtime_error e) {
        lit.clear();
        backrefs->clear();
        for (auto el : tokens) {
            delete el;
        }
        throw e;
    }
}

Alphabet* readCharset(const char** strPointer) {
    const char* input = *strPointer;
    bool negation = false;
    stack<char> chs;

    if (*input != '[') {
        // ERROR not a characterset
        throw syntax_error("Not a character set");
    }
    input++;

    if (*input == '^') {
        negation = true;
    }
    else {
        chs.push(*input);
    }
    input++;
    
    while (*input != ']') {
        if (*input == '\0') {
            // ERROR unmatched [
            throw syntax_error("Unmatched [");
        }
        if (*input == ':' && !chs.empty() && chs.top() == '[') {
            // named class
            chs.pop();
            input++;
            const char* it = input;

            // find end
            while (!(*it == ':' && *(it + 1) == ']')) {
                if (*it == '\0') {
                    // ERROR unmatched [:
                    throw syntax_error("Unmatched [:");
                }
                it++;
            }

            // extract string
            char* cstr = new char[it - input + 1];
            size_t i = 0;
            while (input != it) {
                cstr[i++] = *input++;
            }
            cstr[i] = '\0';
            string str(cstr);
            delete cstr;
            input++;
            // input points to ]

            // find correct string
            if (str == "alnum") {
                addLower(chs);
                addUpper(chs);
                addDigits(chs);
            } else if (str == "alpha") {
                addLower(chs);
                addUpper(chs);
            } else if (str == "blank") {
                chs.push(' ');
                //chs.push('\t');
            } /*else if (str == "cntrl") {
                for (char c = 0; c <= 31; c++) {
                    chs.push(c);
                }
                chs.push(127);
            }*/ else if (str == "digit") {
                addDigits(chs);
            } else if (str == "graph") {
                addLower(chs);
                addUpper(chs);
                addDigits(chs);
                addPunct(chs);
            } else if (str == "lower") {
                addLower(chs);
            } else if (str == "print") {
                addLower(chs);
                addUpper(chs);
                addDigits(chs);
                addPunct(chs);
                chs.push(' ');
            } else if (str == "punct") {
                addPunct(chs);
            } else if (str == "space") {
                addSpace(chs);
            } else if (str == "upper") {
                addUpper(chs);
            } else if (str == "xdigit") {
                addDigits(chs);
                for (char c = 'a'; c <= 'f'; c++) {
                    chs.push(c);
                }
                for (char c = 'A'; c <= 'F'; c++) {
                    chs.push(c);
                }
            } else {
                // ERROR unknown named class
                throw syntax_error("Unknown named class");
            }
            input++;
        }
        else if (*input == '-') {
            // range
            input++;
            if (*input == ']') {
                chs.push('-');
                break;
            }

            char begin = chs.top(), end = *input;
            chs.pop();
            if (begin > end) {
                // ERROR end is before begin
                throw syntax_error("Range end is before start");
            }
            for (char c = begin; c <= end; c++) {
                if (c == 127) continue;
                chs.push(c);
            }
            
            input++;

            // shield against double ranges
            if (*input == '-' && *(input + 1) != ']') {
                // ERROR [a-b-c] invalid
                throw syntax_error("Invalid range");
            }
        }
        else {
            // normal char
            chs.push(*input);
            input++;
        }
        // input is left pointing at first unread char
    }

    set<char> chars;
    while (!chs.empty()) {
        chars.insert(chs.top());
        chs.pop();
    }

    // leave input on trailing ]
    *strPointer = input;

    return new Alphabet(negation, chars);
}