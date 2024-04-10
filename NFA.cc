#include <stack>

#include "NFA.h"
#include "AST.h"

using std::set;
using std::string;
using std::stack;

namespace nfa {

// represents a state in the NFA
struct State {
    Node* state;
    metastring::Metastring string;
};

NFA::NFA(const ast::Node* tree) {
    Module m(tree->buildModule());
    startNode = m.start;
    endNode = m.end;
}

NFA::~NFA() {
    set<Node*> discovered;
    stack<Node*> enqueued;
    discovered.insert(startNode);
    enqueued.push(startNode);
    while (!enqueued.empty()) {
        Node* n = enqueued.top();
        enqueued.pop();
        for (Edge* out : n->outgoing) {
            if (discovered.insert(out->dest).second) {
                enqueued.push(out->dest);
            }
        }
    }
    for (Node* n : discovered) delete n;
}

set<string>* NFA::evaluate() const {
    // TODO
}

}  // namespace nfa