#include "Ast.h"

using std::string;
using std::set;

set<Metastring> LiteralNode::interpret() {

}

set<Metastring> CharsetNode::interpret() {

}

set<Metastring> RepeatNode::interpret() {

}

RepeatNode::~RepeatNode() {
    delete child;
}

set<Metastring> OpenNode::interpret() {

}

OpenNode::~OpenNode() {
    delete child;
}

set<Metastring> CloseNode::interpret() {

}

CloseNode::~CloseNode() {
    delete child;
}

set<Metastring> BackrefNode::interpret() {

}

BackrefNode::~BackrefNode() {
    delete child;
}

InfixNode::~InfixNode() {
    delete ln;
    delete rn;
}

set<Metastring> OrNode::interpret() {

}

set<Metastring> EdgeNode::interpret() {

}