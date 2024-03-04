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

set<Metastring> GroupNode::interpret() {
    set<Metastring> ch(child->interpret());
    for (auto& ms : ch) {
        const_cast<Metastring&>(ms).enableBackref(backref);
    }
    return ch;
}

GroupNode::~GroupNode() {
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

set<Metastring> OrNode::interpret() {
    set<Metastring> rs(ln->interpret());
    rs.merge(rn->interpret());
    return rs;
}

OrNode::~OrNode() {
    delete ln;
    delete rn;
}