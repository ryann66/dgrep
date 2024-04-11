#include <stdexcept>

#include "AST.h"
#include "truncation_error.h"

using std::string;
using std::set;
using std::vector;
using std::logic_error;

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

/**
 * Returns a module that repeats the given module exactly n times
 * Uses the given module
 * Warning: the module should have no outgoing edges from m.end (they will be duplicated)
*/
nfa::Module fixed_series(Module m, unsigned int n) {
    if (n == 0) throw new logic_error("Finite series 0 length: memory leak");
    if (n == 1) {
        return m;
    }
    Module base(m.duplicate());
    for (unsigned int i = 1; i < n - 1; i++) {
        // expand base with m
        Module dup(m.duplicate());
        base.end->outgoing.push_back(new nfa::Edge(dup.start));
        base.end = dup.end;
    }
    // stitch m onto end of base
    base.end->outgoing.push_back(new nfa::Edge(m.start));
    base.end = m.end;
    return base;
}

/**
 * Returns a module that repeats the given module
 * between 1 and n times
 * Uses the given module
*/
nfa::Module finite_series(Module m, unsigned int n) {
    nfa::Node* dest = new nfa::Node();
    m.end->outgoing.push_back(new nfa::Edge(dest));
    Module r(fixed_series(m, n));
    r.end = dest;
    return r;
}

/**
 * Returns a module that repeats the given module at least once
 * (up to infinity)
 * Uses the given module
*/
nfa::Module infinite_series(Module m) {
    m.end->outgoing.push_back(new nfa::Edge(m.start));
    return m;
}

nfa::Module RepeatNode::buildModule() const {
    unsigned int low = res.lowerBound, high = res.upperBound;
    Module base = child->buildModule();
    if (!base.end->outgoing.empty()) {
        nfa::Node* n = new nfa::Node();
        base.end->outgoing.push_back(new nfa::Edge(n));
        base.end = n;
    }
    if (low == 0) {
        if (high == INFINITE_REPEAT) {
            base = infinite_series(base);
        } else {
            base = finite_series(base, high);
        }
        // add bypass
        base.start->outgoing.push_back(new nfa::Edge(base.end));
        return base;
    }
    if (low == high) {
        return fixed_series(base, low);
    }
    Module fixedPart;
    if (low != 1) {
        fixedPart = fixed_series(base.duplicate(), low - 1);
    }
    if (high == INFINITE_REPEAT) {
        base = infinite_series(base);
    } else {
        base = finite_series(base, high - low + 1);
    }
    if (low != 1) {
        // stitch
        fixedPart.end->outgoing.push_back(new nfa::Edge(base.start));
        return Module(fixedPart.start, base.end);
    } else {
        return base;
    }
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
    Module rightInner(rn->buildModule());
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
    if (children.empty()) {
        nfa::Node* n = new nfa::Node();
        return Module(n, n);
    }
    auto iter = children.begin();
    Module m((*iter)->buildModule());
    nfa::Node* start = m.start;
    nfa::Node* end = m.end;
    iter++;
    while (iter != children.end()) {
        m = (*iter)->buildModule();
        end->outgoing.push_back(new nfa::Edge(m.start));
        end = m.end;
    }
    return Module(start, end);
}

}  // namespace ast