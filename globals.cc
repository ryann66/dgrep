#include "globals.h"

using std::set;

set<char> initAlphabet() {
    set<char> alph;
    for (char c = 32; c < 127; c++) alph.insert(c);
    return alph;
}