#pragma once

#include <set>

// maximum number backref that will be encountered
extern unsigned char maxBackref = 0;

// maximum length of matched strings, including null character
extern size_t maxLength = 256;

const extern std::set<char> alphabet(initAlphabet());

/**
 * initializes the alphabet to be all 'standard' ASCII characters
 * NOTE: standard ASCII characters are letters, numbers, symbols, and space
 *       this excludes characters like tab, as well as all control characters
*/
std::set<char> initAlphabet();