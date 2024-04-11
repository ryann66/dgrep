#include <stdexcept>

#include "Alphabet.h"
#include "globals.h"

using std::set;
using std::range_error;
using std::string;

Alphabet alphabet;

Alphabet::Alphabet() {
    // Default: all printable characters
    for (char c = 32; c < 127; c++) chars.insert(c);
}

Alphabet::Alphabet(bool isNegation, const set<char>& chs) {
    if (isNegation) {
        chars = alphabet.chars;
        for (char c : chs) {
            if (chars.erase(c) == 0) {
                // ERROR c is not in alphabet
                throw range_error("'" + string(1,c) + "' is not in alphabet");
            }
        }
    } else {
        chars = chs;
        for (char c : chars) {
            if (alphabet.chars.find(c) == alphabet.end()) {
                // ERROR c is not in alphabet
                throw range_error("'" + string(1,c) + "' is not in alphabet");
            }
        }
    }
}
