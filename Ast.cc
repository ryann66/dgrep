#include "Ast.h"
#include "truncation_error.h"

using std::string;
using std::set;
using std::vector;

set<string> Node::evaluate() {
    set<Metastring> res(this->interpret());
    set<string> ret;
    for (auto& ms : res) {
        ret.insert(ms.toString());
    }
    return ret;
}

set<Metastring> LiteralNode::interpret() {
    set<Metastring> r;
    r.emplace(this->lit);
}

set<Metastring> CharsetNode::interpret() {
    set<Metastring> r;
    for (char c : chs.chars) r.emplace(c);
}

set<Metastring> RepeatNode::interpret() {
    set<Metastring> toks(child->interpret());
    if (toks.empty()) return toks;

    set<Metastring> accepted, iterate, resulted;
    unsigned int lb = res.lowerBound, ub = res.upperBound;
    iterate.emplace("");

    // build minimum strings
    for (unsigned int i = 0; i < lb && !iterate.empty(); i++) {
        // append each token onto all roots in iterate
        for (auto& ms : iterate) {
            for (auto& tok : toks) {
                try {
                    accepted.insert(ms + tok);
                } catch (truncation_error e) { }
            }
        }
        // move the roots into iterate
        iterate.swap(resulted);
        resulted.clear();
    }
    
    // mark all minimum strings accepted
    for (auto& res : iterate) accepted.insert(res);

    // expand string
    for (unsigned int i = lb; i < ub && !iterate.empty(); i++) {
        // append each token onto all roots in iterate
        for (auto& ms : iterate) {
            for (auto& tok : toks) {
                try {
                    accepted.insert(ms + tok);
                } catch (truncation_error e) { }
            }
        }
        // mark results accepted
        for (auto& res : resulted) accepted.insert(res);
        // move the roots into iterate
        iterate.swap(resulted);
        resulted.clear();
    }

    return accepted;
}

RepeatNode::~RepeatNode() {
    delete child;
}

set<Metastring> OpenNode::interpret() {
    set<Metastring> ch(child->interpret());
    for (auto& ms : ch) {
        const_cast<Metastring&>(ms).enableBackref(backref);
    }
    return ch;
}

OpenNode::~OpenNode() {
    delete child;
}

set<Metastring> CloseNode::interpret() {
    set<Metastring> ch(child->interpret());
    for (auto& ms : ch) {
        const_cast<Metastring&>(ms).disableBackref(backref);
    }
    return ch;
}

CloseNode::~CloseNode() {
    delete child;
}

set<Metastring> BackrefNode::interpret() {
    set<Metastring> ch(child->interpret());
    set<Metastring> rs;
    for (auto& ms : ch) {
        try {
            rs.insert(const_cast<Metastring&>(ms).appendBackref(backref));
        } catch (truncation_error e) { }
    }
    return rs;
}

BackrefNode::~BackrefNode() {
    delete child;
}

InfixNode::~InfixNode() {
    delete ln;
    delete rn;
}

set<Metastring> OrNode::interpret() {
    set<Metastring> rs(ln->interpret());
    rs.merge(rn->interpret());
    return rs;
}

bool isword(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool startWordPredicate(const Metastring* l, const Metastring* r) {
    return (!isword(l->lastChar())) && isword(r->firstChar());
}

bool endWordPredicate(const Metastring* l, const Metastring* r) {
    return isword(l->lastChar()) && (!isword(r->firstChar()));
}

bool edgePredicate(const Metastring* l, const Metastring* r) {
    return isword(l->lastChar()) ^ isword(r->firstChar());
}

bool notEdgePredicate(const Metastring* l, const Metastring* r) {
    return (!isword(l->lastChar()) ^ isword(r->firstChar()));
}

set<Metastring> EdgeNode::interpret() {
    set<Metastring> acc;
    bool (*predicate)(const Metastring*, const Metastring*);
    switch (t) {
        case StartWord:
            predicate = startWordPredicate;
            break;
        case EndWord:
            predicate = endWordPredicate;
            break;
        case IsEdge:
            predicate = edgePredicate;
            break;
        case NotEdge:
            predicate = notEdgePredicate;
            break;
    }

    if (ln && rn) {
        set<Metastring> ls(ln->interpret()), rs(rn->interpret());
        for (auto& l : ls) {
            for (auto& r : rs) {
                if (predicate(&l, &r)) {
                    try {
                        acc.insert(l + r);
                    } catch (truncation_error e) { }
                }
            }
        }
    } else if (ln) {
        // rn null
        set<Metastring> ls(ln->interpret());
        Metastring r(' ');
        for (auto& l : ls) {
            if (predicate(&l, &r)) {
                acc.insert(l);
            }
        }
    } else if (rn) {
        // ln null
        set<Metastring> rs(rn->interpret());
        Metastring l(' ');
        for (auto& r : rs) {
            if (predicate(&l, &r)) {
                acc.insert(r);
            }
        }
    } else {
        // logical error: both ln and rn are null
        return *reinterpret_cast<set<Metastring>*>(0x0);
    }
    return acc;
}

set<Metastring> ConcatNode::interpret() {
    if (children.empty()) return set<Metastring>();
    auto chIter = children.begin();
    set<Metastring> roots((*chIter)->interpret()), tmp;

    for (chIter++; chIter != children.end(); chIter++) {
        for (auto& ms : roots) {
            for (auto& mscat : (*chIter)->interpret()) {
                try {
                    tmp.insert(ms + mscat);
                } catch (truncation_error e) { }     
            }
        }
        roots.swap(tmp);
        tmp.clear();
    }

    return roots;
}

ConcatNode::~ConcatNode() {
    for (auto p : children) delete p;
}