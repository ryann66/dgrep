#include "AST.h"
#include "truncation_error.h"

using std::string;
using std::set;
using std::vector;

using metastring::Metastring;
using nfa::Module;

namespace ast {

nfa::Module LiteralNode::buildModule() const {
    nfa::Node* start = new nfa::Node();
    nfa::Node* end = new nfa::Node();
    start->outgoing.push_back(new nfa::StringEdge(end, lit));
    return Module(start, end);
}

nfa::Module CharsetNode::buildModule() const {
    nfa::Node* start = new nfa::Node();
    nfa::Node* end = new nfa::Node();
    for (char c : chs) {
        start->outgoing.push_back(new nfa::CharEdge(end, c));
    }
    return Module(start, end);
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
    nfa::Node* start = new nfa::Node();
    nfa::Node* end = new nfa::Node();
    Module inner(child->buildModule());
    start->outgoing.push_back(new nfa::StartAppendEdge(inner.start, backref));
    inner.end->outgoing.push_back(new nfa::EndAppendEdge(end, backref));
    return Module(start, end);
}

nfa::Module BackrefNode::buildModule() const {
    nfa::Node* start = new nfa::Node();
    nfa::Node* end = new nfa::Node();
    start->outgoing.push_back(new nfa::BackrefEdge(end, backref));
    return Module(start, end);
}

OrNode::~OrNode() {
    delete ln;
    delete rn;
}

nfa::Module OrNode::buildModule() const {
    nfa::Node* start = new nfa::Node();
    nfa::Node* end = new nfa::Node();
    Module leftInner(ln->buildModule());
    start->outgoing.push_back(new nfa::Edge(leftInner.start));
    leftInner.end->outgoing.push_back(new nfa::Edge(end));
    Module rightInner(ln->buildModule());
    start->outgoing.push_back(new nfa::Edge(rightInner.start));
    rightInner.end->outgoing.push_back(new nfa::Edge(end));
    return Module(start, end);
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