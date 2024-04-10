#include <vector>
#include <algorithm>

#include "Parser.h"
#include "syntax_error.h"

using std::vector;
using std::find_if;
using namespace ast;

/**
 * Parses the tokens between the iterators (inclusive begin, exclusive end), and returns an AST node
 * Uses a recursive descent style
 * if the result would be empty, then nullptr is returned
*/
Node* recursiveParse(vector<Token*>::const_iterator begin, vector<Token*>::const_iterator end);

Node* parseTokensToAST(const vector<Token*>& tokens) {
    if (tokens.empty()) throw syntax_error("Empty regex");
    Node* root = recursiveParse(tokens.begin(), tokens.end());
    if (root == nullptr) throw syntax_error("Empty regex");
    return root;
}

Node* recursiveParse(vector<Token*>::const_iterator begin, vector<Token*>::const_iterator end) {
    vector<Node*> result;
    while (begin != end) {
        switch ((*begin)->type) {
            case Literal:{
                result.push_back(new LiteralNode(*static_cast<LiteralToken*>(*begin)));
                break;}
            case Charset:{
                result.push_back(new CharsetNode(*static_cast<CharsetToken*>(*begin)));
                break;}
            case Repeat:{
                if (result.empty()) throw syntax_error("Leading repeat token");
                Node* last = result.back();
                result.pop_back();
                result.push_back(new RepeatNode(*static_cast<RepeatToken*>(*begin), last));
                break;}
            case Or:{
                if (result.empty()) throw syntax_error("Leading or token");
                Node* lhs = result.size() == 1 ? result[0] : new ConcatNode(result);
                Node* rhs = recursiveParse(++begin, end);
                if (rhs == nullptr) throw syntax_error("Trailing or token");
                return new OrNode(lhs, rhs);}
            case OpenGroup:{
                unsigned char gn = static_cast<GroupToken*>(*begin)->groupNumber;
                vector<Token*>::const_iterator endIter = find_if(begin, end, [gn](Token* t){ return t->type == CloseGroup && static_cast<GroupToken*>(t)->groupNumber == gn; });
                if (endIter == end) throw syntax_error("Unmatched Opengroup");
                Node* groupContents = recursiveParse(begin + 1, endIter);
                begin = endIter;
                if (groupContents == nullptr) groupContents = new LiteralNode("");
                result.push_back(new GroupNode(gn, groupContents));
                break;}
            case CloseGroup:
                throw syntax_error("Unmatched closegroup");
            case Backref:{
                unsigned char grn = static_cast<GroupToken*>(*begin)->groupNumber;
                result.push_back(new BackrefNode(grn));
                break;}
        }
        begin++;
    }
    
    if (result.empty()) return nullptr;
    if (result.size() == 1) return result[0];
    return new ConcatNode(result);
}