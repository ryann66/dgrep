#pragma once

#include <vector>
#include <set>
#include <string>

#include "Tokens.h"
#include "Alphabet.h"

/**
 * Returns a vector of tokens, in order from front to back
 * throws a runtime_error* to indicate failure, includes definition of error
 * Backrefs is an output pointer to a set that will have all used backreferences added to it
 * NOTE: Caller is responsible for freeing all tokens in the vector
*/
std::vector<Token*> tokenize(std::string& str, std::set<unsigned char>* backrefs);

/**
 * Reads a character set from the string pointed to by strPointer
 * On success, leaves the string pointer pointing to the closing ] of the character set
 *  returns the parsed character set
 * On failure, throws runtime_error* and strPointer is undefined
 * NOTE: Caller is responsible for freeing returned alphabet
*/
Alphabet* readCharset(const char** strPointer);