#include <cstdlib>
#include <cerrno>
#include <algorithm>

#include "Tokenizer.h"

using std::find;

string lit;

/**
 * Clears out the lit variable and returns the resultant token
*/
Token* flushLit() {
    Token* t = new LiteralToken(lit);
    lit.clear();
    return t;
}

vector<Token*> tokenize(string& str) {
    vector<Token*> tokens;
    const char* input = str.c_str();
    bool fail = false;
    unsigned char maxGroupNum = 0;
    vector<unsigned char> openGroups;  // iterable stack

    while (*input) {
        switch(*input) {
            case '(':  // open
                flushLit();
                tokens.push_back(new GroupToken(OpenGroup, ++maxGroupNum));
                openGroups.push_back(maxGroupNum);
                break;
            case ')':  // close
                if (openGroups.empty()) {
                    // ERROR unmatched )
                    fail = true;
                    break;
                }
                flushLit();
                tokens.push_back(new GroupToken(CloseGroup, openGroups.back()));
                openGroups.pop_back();
                break;
            case '{':  // arb repeat start
                input++;
                // either number or comma
                long num;
                // {,num}
                if (*input == ',') {
                    num = strtol(input, const_cast<char**>(&input), 10);
                    if (num == 0 && errno == EINVAL) {
                        // ERROR {, must be followed by a number
                        fail = true;
                        break;
                    }
                    if (num < 0) {
                        // ERROR negative argument
                        fail = true;
                    }
                    else if (num > INFINITE_REPEAT) {
                        // ERROR argument is too large
                        fail = true;
                    }
                    if (*input != '}') {
                        // ERROR expected }
                        input--;
                        fail = true;
                        break;
                    }
                    flushLit();
                    tokens.push_back(new RepeatToken(0, num));
                    break;
                }
                num = strtol(input, const_cast<char**>(&input), 10);
                if (num == 0 && errno == EINVAL) {
                    // ERROR { must be followed by a number or comma
                    fail = true;
                    break;
                }
                if (num < 0) {
                    // ERROR negative argument
                    fail = true;
                }
                else if (num > INFINITE_REPEAT) {
                    // ERROR argument is too large
                    fail = true;
                }
                // {num}
                if (*input == '}') {
                    flushLit();
                    tokens.push_back(new RepeatToken(num, num));
                    break;
                }
                if (*input == ',') {
                    input++;
                    // {num,}
                    if (*input == '}') {
                        flushLit();
                        tokens.push_back(new RepeatToken(num, INFINITE_REPEAT));
                        break;
                    }
                    // {num,num}
                    long num2 = strtol(input, const_cast<char**>(&input), 10);
                    if (num2 == 0 && errno == EINVAL) {
                        // ERROR {num, must be followed by a number or }
                        fail = true;
                        break;
                    }
                    if (num2 < 0) {
                        // ERROR negative argument
                        fail = true;
                    }
                    else if (num2 > INFINITE_REPEAT) {
                        // ERROR argument is too large
                        fail = true;
                    }
                    if (*input == '}') {
                        flushLit();
                        tokens.push_back(new RepeatToken(num, num2));
                        break;
                    }
                    // ERROR expected } after {num,num
                    fail = true;
                    input--;
                    break;
                }
                // ERROR {num expected to be followed by , or }
                fail = true;
                input--;
                break;
            case '[':  // charset start
                flushLit();
                // TODO
                break;
            case '|':  // or
                flushLit();
                tokens.push_back(new Token(Or));
                break;
            case '?':  // zero or one
                flushLit();
                tokens.push_back(new RepeatToken(0, 1));
                break;
            case '*':  // any number
                flushLit();
                tokens.push_back(new RepeatToken(0, INFINITE_REPEAT));
                break;
            case '+':  // at least one
                flushLit();
                tokens.push_back(new RepeatToken(1, INFINITE_REPEAT));
                break;
            case '\\':
                input++;
                switch(*input) {
                    case 'b':
                        // edge of word
                        flushLit();
                        tokens.push_back(new EdgeToken(IsEdge));
                        break;
                    case 'B':
                        // not edge of word
                        flushLit();
                        tokens.push_back(new EdgeToken(NotEdge));
                        break;
                    case '<':
                        // start of word
                        flushLit();
                        tokens.push_back(new EdgeToken(StartWord));
                        break;
                    case '>':
                        // end of word
                        flushLit();
                        tokens.push_back(new EdgeToken(EndWord));
                        break;
                    case 'w':
                        // alpha or space
                        flushLit();
                        // TODO
                        break;
                    case 'W':
                        // not alpha or space
                        flushLit();
                        // TODO
                        break;
                    case 's':
                        // space
                        flushLit();
                        // TODO
                        break;
                    case 'S':
                        // non space
                        flushLit();
                        // TODO
                        break;
                    case '*':
                    case '+':
                    case '?':
                    case '{':
                    case '[':
                    case '(':
                    case '}':
                    case ']':
                    case ')':
                    case '|':
                    case '\\':
                        lit.push_back(*input);
                    case '\0':
                        // ERROR trailing escape
                        goto fail;
                    default:
                        if (*input > 48 && *input < 58) {
                            // digit (backref)
                            unsigned char n = *input - 48;
                            if (n > maxGroupNum || find(openGroups.begin(), openGroups.end(), n) != openGroups.end()) {
                                // ERROR group is currently open
                                fail = true;
                                break;
                            }
                            flushLit();
                            tokens.push_back(new GroupToken(Backref, n));
                            break;
                        }
                        // ERROR unknown escape character
                        fail = true;
                        break;
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
        fail = true;
    }
    if (fail) {
        lit.clear();
        goto fail;
    }
    tokens.push_back(flushLit());
    return tokens;
fail:
    for (auto el : tokens) {
        delete el;
    }
    vector<Token*> r;
    return r;
}