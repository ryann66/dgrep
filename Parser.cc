#include <vector>

#include "Parser.h"

using std::vector;

/**
 * Parses the tokens between the iterators (inclusive begin, exclusive end), and returns an AST node
*/
Node* parse(vector<Token*>::const_iterator& begin, vector<Token*>::const_iterator& end);

Node* parseTokens(const vector<Token*>& tokens) {
    return parse(tokens.begin(), tokens.end());
}

Node* parse(vector<Token*>::const_iterator& begin, vector<Token*>::const_iterator& end) {
    
}