#pragma once

#include <vector>
#include <set>
#include <string>

#include "Tokens.h"

/**
 * Returns a vector of tokens, in order from front to back
 * Inserts a Concat token between all adjacent values, and after any repeats
 * returns an empty vector on failure
 * Backrefs is an output pointer to a set that will have all used backreferences added to it
 * NOTE: Caller is responsible for freeing all tokens in the vector
*/
std::vector<Token*> tokenize(std::string& str, std::set<unsigned char> backrefs);