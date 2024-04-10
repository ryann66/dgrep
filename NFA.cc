#include "NFA.h"

using std::set;
using std::string;

namespace nfa {

// represents a state in the NFA
struct State {
    Node* state;
    metastring::Metastring string;
};

NFA::NFA(const ast::Node* tree) {
    // TODO
}

NFA::~NFA() {
    // TODO
}

set<string>* NFA::evaluate() const {
    // TODO
}

}  // namespace nfa