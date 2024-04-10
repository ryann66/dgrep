#include "AST.h"
#include "truncation_error.h"

using std::string;
using std::set;
using std::vector;

using metastring::Metastring;

namespace ast {

nfa::Module LiteralNode::buildModule() const {
    // TODO
}

nfa::Module CharsetNode::buildModule() const {
    // TODO
}

RepeatNode::~RepeatNode() {
    delete child;
}

nfa::Module RepeatNode::buildModule() const {
    // TODO
}

GroupNode::~GroupNode() {
    delete child;
}

nfa::Module GroupNode::buildModule() const {
    // TODO
}

nfa::Module BackrefNode::buildModule() const {
    // TODO
}

OrNode::~OrNode() {
    delete ln;
    delete rn;
}

nfa::Module OrNode::buildModule() const {
    // TODO
}

ConcatNode::~ConcatNode() {
    for (Node* n : children) {
        delete n;
    }
}

nfa::Module ConcatNode::buildModule() const {
    // TODO
}

}  // namespace ast