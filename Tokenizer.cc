#include "Tokenizer.h"

// TODO fill out logic
vector<unique_ptr<Token>> tokenize(string& str) {
    vector<unique_ptr<Token>> tokens;
    const char* input = str.c_str();
    bool fail = false;

    while (*input) {
        switch(*input) {
            case '(':  // open
                break;
            case ')':  // close
                break;
            case '{':  // arb repeat start
                break;
            case '[':  // charset start
                break;
            case '|':  // or
                break;
            case '?':  // zero or one
                break;
            case '*':  // any number
                break;
            case '+':  // at least one
                break;
            case '\\':
                input++;
                switch(*input) {
                    case 'b':
                        // edge of word
                        break;
                    case 'B':
                        // not edge of word
                        break;
                    case '<':
                        // start of word
                        break;
                    case '>':
                        // end of word
                        break;
                    case 'w':
                        // alpha or space
                        break;
                    case 'W':
                        // not alpha or space
                        break;
                    case 's':
                        // space
                        break;
                    case 'S':
                        // non space
                        break;
                    case '*':
                        break;
                    case '+':
                        break;
                    case '?':
                        break;
                    case '{':
                        break;
                    case '[':
                        break;
                    case '(':
                        break;
                    case '}':
                        break;
                    case ']':
                        break;
                    case ')':
                        break;
                    case '|':
                        break;
                    case '\\':
                        break;
                    case '\0':
                        // error, trailing escape
                        goto fail;
                    default:
                        if (*input > 48 && *input < 58) {
                            // digit / backref
                            char n = *input - 48;
                            break;
                        }
                        // error unknown escape character
                        fail = true;
                        break;
                }
                break;
            default:
                break;
        }
        input++;
    }

    if (fail) goto fail;
    return tokens;
fail:
    vector<unique_ptr<Token>> r;
    return r;
}