#pragma once

#include <set>

#include "Metastring.h"

/**
 * Abstract class/interface for AST nodes
*/
class Node {
    Node() = delete;
    Node(const Node&) = delete;
    virtual Node& operator=(const Node&) = delete;

    virtual std::set<Metastring> interpret() = 0;
};