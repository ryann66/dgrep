#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Tokens.h"

using std::vector;
using std::unique_ptr;
using std::string;

/**
 * Returns a vector of tokens, in order from front to back
 * Inserts a Concat token between all adjacent values, and after any repeats
 * returns an empty vector on failure
 * NOTE: Caller is responsible for freeing all tokens in the vector
*/
vector<Token*> tokenize(string& str);