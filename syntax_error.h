#pragma once

#include <stdexcept>
#include <string>

class syntax_error : public std::runtime_error {
 public:
    syntax_error(const char* what) : runtime_error(what) { }
    syntax_error(const std::string& what) : runtime_error(what) { }
};