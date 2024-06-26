#pragma once

#include <stdexcept>
#include <string>

#include "globals.h"

class syntax_error : public std::runtime_error {
 public:
    syntax_error(const char* what) : runtime_error(what) { }
    syntax_error(const std::string& what) : runtime_error(what) { }
    virtual ~syntax_error() = default;
};

class trivial_error : public std::runtime_error {
 public:
    trivial_error(const char* s) : runtime_error(s) { }
    trivial_error(std::string s) : runtime_error(s) { }
    virtual ~trivial_error() = default;
};

class nonmatching_error : public trivial_error {
 public:
    nonmatching_error(const char* s) : trivial_error(s) { }
    nonmatching_error(std::string s) : trivial_error(s) { }
    virtual ~nonmatching_error() = default;
};

class truncation_error : public trivial_error {
 public:
    truncation_error() : trivial_error("truncated") {
        truncationCount++;
    }
    virtual ~truncation_error() = default;
};