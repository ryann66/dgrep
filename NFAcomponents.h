#pragma once

#include <vector>
#include <string>

namespace nfa {

struct Edge;

// Node in the NFA
struct Node {
    std::vector<Edge*> outgoing;
};

// Edge in the NFA
struct Edge {
    Node* dest;
};

struct Module {
    Node* start;
    Node* end;

    Module duplicate() const;
};

struct CharEdge : public Edge {
    char c;
};

struct StringEdge : public Edge {
    std::string s;
};

struct BackrefEdge : public Edge {
    unsigned char br;
};

}  // namespace nfa