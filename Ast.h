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
    virtual std::set<metastring::Metastring> interpret() = 0;

    std::set<std::string> evaluate();

 protected:
    Node() = default;
    Node(const Node&) = default;
    virtual Node& operator=(const Node&) = default;
};

class LiteralNode : public Node {
 public:
    LiteralNode(const LiteralToken& token) : lit(token.lit) { }
	LiteralNode(const char* str) : lit(str) { }
    LiteralNode(const LiteralNode&) = default;
    virtual LiteralNode& operator=(const LiteralNode&) = default;

    virtual std::set<metastring::Metastring> interpret();

 private:
    std::string lit;
};

class CharsetNode : public Node {
 public:
    CharsetNode(const CharsetToken& token) : chs(token) { }
    CharsetNode(const CharsetNode&) = default;
    virtual CharsetNode& operator=(const CharsetNode&) = default;

    virtual std::set<metastring::Metastring> interpret();

 private:
    const CharsetToken chs;
};

class RepeatNode : public Node {
 public:
    RepeatNode(const RepeatToken& token, Node* ch) : child(ch), res(token) { }
    RepeatNode(const RepeatNode&) = default;
    virtual RepeatNode& operator=(const RepeatNode&) = default;
    virtual ~RepeatNode();

    virtual std::set<metastring::Metastring> interpret();

 private:
    Node* child;
    const RepeatToken res;
};

class GroupNode : public Node {
 public:
    GroupNode(unsigned char br, Node* ch) : backref(br), child(ch) { }
    GroupNode(const GroupNode&) = default;
    virtual GroupNode& operator=(const GroupNode&) = default;
    virtual ~GroupNode();

    virtual std::set<metastring::Metastring> interpret();

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

    virtual std::set<metastring::Metastring> interpret();

 private:
    unsigned char backref;
	Node* child;
};

class OrNode : public Node {
 public:
    OrNode(Node* lhs, Node* rhs) : ln(lhs), rn(rhs) { }
    OrNode(const OrNode&) = default;
    virtual OrNode& operator=(const OrNode&) = default;
	virtual ~OrNode();

    virtual std::set<metastring::Metastring> interpret();

 private:
    Node *ln, *rn;
};

class ConcatNode : public Node {
 public:
    ConcatNode(std::vector<Node*> chs) : children(chs) { }
    ConcatNode(const ConcatNode&) = default;
    virtual ConcatNode& operator=(const ConcatNode&) = default;
    virtual ~ConcatNode();

    virtual std::set<metastring::Metastring> interpret();

 private:
    std::vector<Node*> children;
};