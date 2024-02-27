#pragma once

#include <vector>
#include <set>

#include "Ast.h"
#include "Tokens.h"

/**
 * Parses the tokens into an abstract syntax tree
 * NOTE: client is responsible for freeing the returned syntax tree
*/
Node* parseTokens(const std::vector<Token*>& tokens);