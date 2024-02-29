#include <stdexcept>

#include "Tokens.h"
#include "globals.h"

using std::set;
using std::range_error;

CharsetToken::CharsetToken(bool isNeg, const set<char>& chs) : Token(Charset) {
    if (isNeg) {
        chars = alphabet;
        for (char c : chs) {
            if (chars.erase(c) == 0) {
                // ERROR c is not in alphabet
                throw range_error(c + " is not in alphabet");
            }
        }
    } else {
        chars = chs;
        for (char c : chars) {
            if (alphabet.find(c) == alphabet.end()) {
                // ERROR c is not in alphabet
                throw range_error(c + " is not in alphabet");
            }
        }
    }
}

