#pragma once

#include <vector>
#include <string>
#include <stack>

namespace nfa {

struct Node;

// represents a state in the NFA
struct State {
    Node* state;
    metastring::Metastring string;

    explicit State(Node* state, metastring::Metastring& string) : state(state), string(string) { }
};

// Edge in the NFA
struct Edge {
    Node* dest;

    explicit Edge(Node* d) : dest(d) { }

    virtual void traverse(State&, std::stack<State>&);
};

// Node in the NFA
struct Node {
    std::vector<Edge*> outgoing;

    ~Node();
};

struct Module {
    Node* start;
    Node* end;

    Module() { }
    explicit Module(Node* src, Node* dst) : start(src), end(dst) { }

    // Forms a deep copy of the module
    // NOTE: end may have edges pointing out of the module that will be shallow copied
    Module duplicate() const;
};

struct CharEdge : public Edge {
    char c;

    explicit CharEdge(Node* d, char c) : Edge(d), c(c) { }

    virtual void traverse(State&, std::stack<State>&);
};

struct StringEdge : public Edge {
    std::string s;

    explicit StringEdge(Node* d, const std::string& s) : Edge(d), s(s) { }

    virtual void traverse(State&, std::stack<State>&);
};

struct BackrefEdge : public Edge {
    // NOTE: this struct must have no more than one outgoing edge!
    unsigned char br;

    explicit BackrefEdge(Node* d, unsigned char br) : Edge(d), br(br) { }

    virtual void traverse(State&, std::stack<State>&);
};

struct StartAppendEdge : public Edge {
    // NOTE: this struct must have no more than one outgoing edge!
    unsigned char br;

    explicit StartAppendEdge(Node* d, unsigned char br) : Edge(d), br(br) { }

    virtual void traverse(State&, std::stack<State>&);
};

struct EndAppendEdge : public Edge {
    // NOTE: this struct must have no more than one outgoing edge!
    unsigned char br;

    explicit EndAppendEdge(Node* d, unsigned char br) : Edge(d), br(br) { }

    virtual void traverse(State&, std::stack<State>&);
};

}  // namespace nfa