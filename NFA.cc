#include <stack>
#include <unordered_map>

#include "NFA.h"
#include "truncation_error.h"

using std::set;
using std::string;
using std::stack;
using std::unordered_map;
using metastring::Metastring;

namespace nfa {

void Edge::traverse(State& state, std::stack<State>& res) {
    // does nothing to string, only advances
    res.emplace(dest, state.string);
}

Node::~Node() {
    for (Edge* e : outgoing) delete e;
}

Module Module::duplicate() const {
    // find all nodes
    set<Node*> discovered;
    stack<Node*> enqueued;
    discovered.insert(start);
    enqueued.push(start);
    while (!enqueued.empty()) {
        Node* n = enqueued.top();
        enqueued.pop();
        if (n == end) continue;  // block from exiting module
        for (Edge* out : n->outgoing) {
            if (discovered.insert(out->dest).second) {
                enqueued.push(out->dest);
            }
        }
    }
    
    unordered_map<Node*, Node*> conversion;
    for (Node* n : discovered) {
        // create a new node for all nodes in module
        conversion[n] = new Node();
    }
    for (Edge* e : end->outgoing) {
        // preserve the output node's destination
        conversion[e->dest] = e->dest;
    }

    // stitch together new module
    for (Node* oldNode : discovered) {
        Node* newNode = conversion[oldNode];
        newNode->outgoing.reserve(oldNode->outgoing.size());
        for (Edge* e : oldNode->outgoing) {
            Edge* newE = new Edge(conversion[e->dest]);
            newNode->outgoing.emplace_back();
        }
    }
    return Module(conversion[start], conversion[end]);
}

void CharEdge::traverse(State& state, stack<State>& res) {
    // append c onto the new state
    Metastring m(state.string + c);
    res.emplace(dest, m);
}

void StringEdge::traverse(State& state, stack<State>& res) {
    // append s onto the new state
    Metastring m(state.string + s);
    res.emplace(dest, m);
}

void BackrefEdge::traverse(State& state, stack<State>& res) {
    // append backref onto the new state but otherwise preserve string
    state.string.appendBackref(br);
    res.emplace(dest, state.string);
}

void StartAppendEdge::traverse(State& state, stack<State>& res) {
    // append backref onto the new state but otherwise preserve string
    state.string.startBackrefLogging(br);
    res.emplace(dest, state.string);
}

void EndAppendEdge::traverse(State& state, stack<State>& res) {
    // append backref onto the new state but otherwise preserve string
    state.string.endBackrefLogging(br);
    res.emplace(dest, state.string);
}

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
    set<string>* ret = new set<string>();
    stack<State> states;
    Metastring m("");
    states.emplace(startNode, m);
    while (!states.empty()) {
        State s(states.top());
        states.pop();
        if (s.state == endNode) {
            ret->insert(s.string.toString());
            continue;
        }
        for (Edge* e : s.state->outgoing) {
            try {
                e->traverse(s, states);
            } catch (truncation_error*) { }
        }
    }
    return ret;
}

}  // namespace nfa