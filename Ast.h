#pragma once

#include <set>
#include <string>
#include <vector>

#include "Metastring.h"
#include "Tokens.h"

/**
 * Abstract class/interface for AST nodes
*/
class Node {
 public:
    virtual std::set<Metastring> interpret() = 0;

    std::set<std::string> evaluate();

 protected:
    Node() = default;
    Node(const Node&) = default;
    virtual Node& operator=(const Node&) = default;
};

class LiteralNode : public Node {
 public:
    LiteralNode(const LiteralToken& token) : lit(token.lit) { }
    LiteralNode(const LiteralNode&) = default;
    virtual LiteralNode& operator=(const LiteralNode&) = default;

    virtual std::set<Metastring> interpret();

 private:
    std::string lit;
};

class CharsetNode : public Node {
 public:
    CharsetNode(const CharsetToken& token) : chs(token) { }
    CharsetNode(const CharsetNode&) = default;
    virtual CharsetNode& operator=(const CharsetNode&) = default;

    virtual std::set<Metastring> interpret();

 private:
    const CharsetToken& chs;
};

class RepeatNode : public Node {
 public:
    RepeatNode(const RepeatToken& token, Node* ch) : child(ch), res(token) { }
    RepeatNode(const RepeatNode&) = default;
    virtual RepeatNode& operator=(const RepeatNode&) = default;
    virtual ~RepeatNode();

    virtual std::set<Metastring> interpret();

 private:
    Node* child;
    const RepeatToken& res;
};

class OpenNode : public Node {
 public:
    OpenNode(unsigned char br, Node* ch) : backref(br), child(ch) { }
    OpenNode(const OpenNode&) = default;
    virtual OpenNode& operator=(const OpenNode&) = default;
    virtual ~OpenNode();

    virtual std::set<Metastring> interpret();

 private:
    unsigned char backref;
    Node* child;
};

class CloseNode : public Node {
 public:
    CloseNode(unsigned char br, Node* ch) : backref(br), child(ch) { }
    CloseNode(const CloseNode&) = default;
    virtual CloseNode& operator=(const CloseNode&) = default;
    virtual ~CloseNode();

    virtual std::set<Metastring> interpret();

 private:
    unsigned char backref;
    Node* child;
};

class BackrefNode : public Node {
 public:
    BackrefNode(unsigned char br, Node* ch) : backref(br), child(ch) { }
    BackrefNode(const BackrefNode&) = default;
    virtual BackrefNode& operator=(const BackrefNode&) = default;
    virtual ~BackrefNode();

    virtual std::set<Metastring> interpret();

 private:
    unsigned char backref;
    Node* child;
};

class InfixNode : public Node {
 public:
    virtual ~InfixNode();

    virtual std::set<Metastring> interpret() = 0;

 protected:
    InfixNode(Node* lhs, Node* rhs) : ln(lhs), rn(rhs) { }
    InfixNode(const InfixNode&) = default;
    virtual InfixNode& operator=(const InfixNode&) = default;

    Node *ln, *rn;
};

class OrNode : public InfixNode {
 public:
    OrNode(Node* lhs, Node* rhs) : InfixNode(lhs, rhs) { }
    OrNode(const OrNode&) = default;
    virtual OrNode& operator=(const OrNode&) = default;

    virtual std::set<Metastring> interpret();
};

/**
 * lhs or rhs can be null (but not both)
*/
class EdgeNode : public InfixNode {
 public:
    EdgeNode(Node* lhs, Node* rhs, const EdgeToken& token) : InfixNode(lhs, rhs), t(token.edgeType) { }
    EdgeNode(const EdgeNode&) = default;
    virtual EdgeNode& operator=(const EdgeNode&) = default;

    virtual std::set<Metastring> interpret();

 private:
    EdgeType t;
};

class ConcatNode : public Node {
 public:
    ConcatNode(std::vector<Node*> chs) : children(chs) { }
    ConcatNode(const ConcatNode&) = default;
    virtual ConcatNode& operator=(const ConcatNode&) = default;
    virtual ~ConcatNode();

    virtual std::set<Metastring> interpret();

 private:
    std::vector<Node*> children;
};