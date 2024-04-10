#pragma once

#include <string>
#include <set>

#include "AST.h"
#include "NFAcomponents.h"

using std::string;

namespace nfa {

// represents the whole NFA
class NFA {
 public:
    // forms an NFA out of the given AST tree
    NFA(const ast::Node* tree);
    NFA() = delete;
    NFA(const NFA&) = delete;
    NFA& operator=(const NFA&) = delete;
    ~NFA();

    // returns the set of strings that match this
    std::set<std::string>* evaluate() const;

 private:
    Node* startNode;
    Node* endNode;
};

}  // namespace nfa