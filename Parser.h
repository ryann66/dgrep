#pragma once

#include <vector>
#include <set>

#include "AST.h"
#include "Tokens.h"

/**
 * Parses the tokens into an abstract syntax tree
 * NOTE: client is responsible for freeing the returned syntax tree
 * NODE: client maintains ownership of all the tokens
*/
ast::Node* parseTokensToAST(const std::vector<Token*>& tokens);