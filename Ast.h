#pragma once

#include <set>
#include <string>

/**
 * Abstract class/interface for AST nodes
*/
class Node {
    Node() = delete;
    Node(const Node&) = delete;
    virtual Node& operator=(const Node&) = delete;

    virtual std::set<std::string> interpret() = 0;
};