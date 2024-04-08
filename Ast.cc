#include "Ast.h"
#include "truncation_error.h"

using std::string;
using std::set;
using std::vector;

using metastring::Metastring;

RepeatNode::~RepeatNode() {
    delete child;
}

GroupNode::~GroupNode() {
    delete child;
}

OrNode::~OrNode() {
    delete ln;
    delete rn;
}

ConcatNode::~ConcatNode() {
    for (Node* n : children) {
        delete n;
    }
}